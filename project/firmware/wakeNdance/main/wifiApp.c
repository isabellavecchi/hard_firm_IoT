/*
 * wifiApp.c
 *
 *  Created on: 16 de nov. de 2024
 *      Author: Luiz Carlos
 */


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries
#include <stdint.h>

// ESP libraries
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_interface.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
#include "esp_wifi_default.h"
#include "esp_wifi_types_generic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/portmacro.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

// Personal libraries
#include "wifiApp.h"
#include "httpServer.h"
#include "ledRGB.h"
#include "tasks_common.h"


/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "wifi_app";

// Wifi connected event callback
//static wifi_connected_event_callback_t wifi_connected_event_cb;

// Alocating Station WiFi credencials
char ssid[WIFI_SSID_LENGTH];
char passwd[WIFI_PASSWORD_LENGTH];

// Used for returning the WiFi configuration
wifi_config_t wifi_config_v;

// Used to track the number for retries when a connectiona attempt fails
static uint8_t g_retry_number;

// Netif objects for the station and access point
esp_netif_t * esp_netif_sta = NULL;
esp_netif_t * esp_netif_ap  = NULL;

// Variable to check if the state Id is valid
static uint8_t g_qtd_states;


	/* FreeRTOS Structure */

// Queue handle used to manipulate the main queue of events
static QueueHandle_t wifi_app_queue_handle_t;


	/* Static Functions */
	
// STATE MACHINE
static void WIFI_STATE_FUNC_NAME(WIFI_APP_START_HTTP_SERVER)(wifi_app_queue_message_t * st);
static void WIFI_STATE_FUNC_NAME(WIFI_APP_CONNECTING_FROM_HTTP_SERVER)(wifi_app_queue_message_t * st);
static void WIFI_STATE_FUNC_NAME(WIFI_APP_STA_CONNECTED_GOT_IP)(wifi_app_queue_message_t * st);
static void wifiApp_stateMachine_handler(wifi_app_queue_message_t * msg);

// STA & AP FUNCTIONS
static void wifiApp_event_handler(void * arg_p, esp_event_base_t event_base, int32_t eventId, void * eventData_p);
static void wifiApp_eventHandler_init(void);
static void wifiApp_defaultWifi_init(void);
static void wifiApp_softAP_config(void);
static void wifiApp_sta_connect(void);
static void wifiApp_sta_disconnectedLogInfo(void * eventData_p);

// APP FUNCTIONS
static void wifiApp_setup(void);
static void wifiApp_task(void * pvParameters);




/**************************
**		  GETTERS		 **
**************************/

/**
 * Returns the Address of the first char of SSID string
 */
char * wifiApp_getStationSSID(void)
{
	return ssid;
}

/**
 * Returns the Address of the first char of password string
 */
char * wifiApp_getStationPassword(void)
{
	return passwd;
}

/**
 * Returns the WiFi configuration Address
 */
 wifi_config_t * wifiApp_getWifiConfig(void)
 {
	 return &wifi_config_v;
 }



/**************************
**		APP FUNCTIONS	 **
**************************/

 /**
  * function that sets up the WiFi environment
  * configurating WiFi & Soft Access and initializing TCP/IP
  */
static void wifiApp_setup(void)
{
	// Initialize the event handler
	wifiApp_eventHandler_init();
	
	// Initialize the TCP/IP stack and WiFi config
	wifiApp_defaultWifi_init();
	
	// SoftAP config
	wifiApp_softAP_config();
}

///**
// * Sets the callback function
// */
// void wifiApp_setCallback(wifi_connected_event_callback_t callbackFunction)
// {
//	 wifi_connected_event_cb = callbackFunction;
// }



/**************************
**		STATE MACHINE	 **
**************************/

/**
 * State Machine Function Definition according to sm_wifi_app_function
 * function that defines the behavior on 
 * [WIFI_APP_START_HTTP_SERVER] state
 */
static void WIFI_STATE_FUNC_NAME(WIFI_APP_START_HTTP_SERVER)(wifi_app_queue_message_t * st)
{
	ESP_LOGI(TAG, "%s", sm_wifi_app_state_names[WIFI_APP_START_HTTP_SERVER]);
	
	httpServer_start();
	ledRGB_wifi_disconnected();
	
//	wifi_connected_event_cb();
}

/**
 * State Machine Function Definition according to sm_wifi_app_function
 * function that defines the behavior on
 * [WIFI_APP_CONNECTING_FROM_HTTP_SERVER] state
 */
static void WIFI_STATE_FUNC_NAME(WIFI_APP_CONNECTING_FROM_HTTP_SERVER)(wifi_app_queue_message_t * st)
{
	ESP_LOGI(TAG, "%s", sm_wifi_app_state_names[WIFI_APP_CONNECTING_FROM_HTTP_SERVER]);
	
	// Attempt a connection
	wifiApp_sta_connect();
	
	// Set current number of retries to zero
	g_retry_number = 0;
	
	// Let the HTTP server know about the connection attempt
	httpServer_monitor_sendMessage(HTTP_WIFI_CONNECT_INIT);
}

/**
 * State Machine Function Definition according to sm_wifi_app_function
 * function that defines the behavior on
 * [WIFI_APP_STA_CONNECTED_GOT_IP] state
 */
static void WIFI_STATE_FUNC_NAME(WIFI_APP_STA_CONNECTED_GOT_IP)(wifi_app_queue_message_t * st)
{
	ESP_LOGI(TAG, "%s", sm_wifi_app_state_names[WIFI_APP_STA_CONNECTED_GOT_IP]);
	
 	ledRGB_wifi_connected();
 	httpServer_monitor_sendMessage(HTTP_WIFI_CONNECT_SUCCESS);
}

/**
 * State Machine Function Definition according to sm_wifi_app_function
 * function that defines the behavior on
 * [WIFI_APP_USER_REQUESTED_STA_DISCONNECT] state
 */
static void WIFI_STATE_FUNC_NAME(WIFI_APP_USER_REQUESTED_STA_DISCONNECT)(wifi_app_queue_message_t * st)
{
	ESP_LOGI(TAG, "%s", sm_wifi_app_state_names[WIFI_APP_STA_DISCONNECTED]);
	
 	ledRGB_wifi_disconnect();
	// so it doesn't try to reconnect when we hit the button disconnect
	g_retry_number = MAX_CONNECTION_RETRIES;
	
 	ESP_ERROR_CHECK(esp_wifi_disconnect());
 	ledRGB_wifi_disconnected();
}

/**
 * State Machine Function Definition according to sm_wifi_app_function
 * function that defines the behavior on
 * [WIFI_APP_STA_DISCONNECTED] state
 */
static void WIFI_STATE_FUNC_NAME(WIFI_APP_STA_DISCONNECTED)(wifi_app_queue_message_t * st)
{
	ESP_LOGI(TAG, "%s", sm_wifi_app_state_names[WIFI_APP_STA_DISCONNECTED]);
	
 	ledRGB_wifi_disconnected();
 	httpServer_monitor_sendMessage(HTTP_WIFI_CONNECT_FAIL);
}


// State Machine Table Functions - to be used in the loop
sm_wifi_table_fn_t sm_wifi_state_table[] =
{
#define X(ID, ENUM) { WIFI_STATE_FUNC_NAME(ENUM) }, 
	X_MACRO_WIFI_STATE_LIST
#undef X
};

static void wifiApp_stateMachine_handler(wifi_app_queue_message_t * msg)
{
	g_qtd_states = 0;
	#define X(ID, ENUM) g_qtd_states++; 
		X_MACRO_WIFI_STATE_LIST
	#undef X
	
	for (;;)
	{
		if (xQueueReceive(wifi_app_queue_handle_t, msg, portMAX_DELAY))
		{
			if(msg->wifiApp_state < g_qtd_states) {
				sm_wifi_state_table[msg->wifiApp_state].func(msg);
			}
		}
	}
}



/**************************
**	FreeRTOS FUNCTIONS	 **
**************************/
 
/**
 * Main task for the WiFi application
 * where we initialize WiFi and and receive all queue messages
 * which determine the flow of the application
 * @param pvParameters parameter which can be passed to the task
 */
static void wifiApp_task(void * pvParameters)
{
	wifi_app_queue_message_t msg;
	
	// Setup WiFi environment
	wifiApp_setup();
	
	// Start WiFi
	ESP_ERROR_CHECK(esp_wifi_start());
	
	// Send first event message
	wifiApp_sendMessage(WIFI_APP_START_HTTP_SERVER);
	
	wifiApp_stateMachine_handler(&msg);
}

/**
 * Sends a message to the queue
 * @param msgId message ID from the wifi_app_message_e enum
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE
 * @TODO colocar um enum status pra indicar falha ou sucesso
 */
BaseType_t wifiApp_sendMessage(sm_wifi_app_state_e msgId)
{
	wifi_app_queue_message_t msg;
	msg.wifiApp_state = msgId;
	return xQueueGenericSend(wifi_app_queue_handle_t, &msg, portMAX_DELAY, queueSEND_TO_BACK );
}

/**
 * Starts the WiFi RTOS task
 */
void wifiApp_start(void)
{
	ESP_LOGI(TAG, "STARTING WIFI APPLICATION");
	
	// Start Wifi started LED
	ledRGB_wifiApp_started();
	
	// Disable default WiFi logging messages
	esp_log_level_set("WiFi", ESP_LOG_NONE);
	
	// Clearing memory for the WiFi configuration
	memset(&wifi_config_v, 0x00, sizeof(wifi_config_t)); 
	
	// Create message queue
	wifi_app_queue_handle_t = xQueueCreate(3, sizeof(wifi_app_queue_message_t));
	
	// Start the WiFi application task
	xTaskCreatePinnedToCore(	&wifiApp_task,
								"wifiApp_task",
								WIFI_APP_TASK_STACK_SIZE,
								NULL,
								WIFI_APP_TASK_PRIORITY,
								NULL,
								WIFI_APP_TASK_CORE_ID);
							
}



/**************************
**	 STA & AP FUNCTIONS	 **
**************************/

/**
 * WiFi application event handler
 * @param arg_p data, aside from event data, that is passed to the handler when it is called
 * @param event_base the base id of the event to register the handler for
 * @param eventId the id of the event to register the handler for
 * @param eventData_p event data
 */
 static void wifiApp_event_handler(void * arg_p, esp_event_base_t event_base, int32_t eventId, void * eventData_p)
 {
	 if (event_base == WIFI_EVENT)
	 {
		 switch (eventId)
		 {
			 case WIFI_EVENT_AP_START:
			 	ESP_LOGI(TAG, "WIFI_EVENT_AP_START");
			 	break;
				
			 case WIFI_EVENT_AP_STOP:
			 	ESP_LOGI(TAG, "WIFI_EVENT_AP_STOP");
			 	break;
				
			 case WIFI_EVENT_AP_STACONNECTED:
			 	ESP_LOGI(TAG, "WIFI_EVENT_AP_STACONNECTED");
			 	break;
				
			 case WIFI_EVENT_AP_STADISCONNECTED:
			 	ESP_LOGI(TAG, "WIFI_EVENT_AP_STADISCONNECTED");
			 	break;
				
			 case WIFI_EVENT_AP_PROBEREQRECVED:
			 	ESP_LOGI(TAG, "WIFI_EVENT_AP_PROBEREQRECVED");
			 	break;
				
			 case WIFI_EVENT_STA_START:
			 	ESP_LOGI(TAG, "WIFI_EVENT_STA_START");
			 	break;
				
			 case WIFI_EVENT_STA_STOP:
			 	ESP_LOGI(TAG, "WIFI_EVENT_STA_STOP");
			 	break;
				
			 case WIFI_EVENT_STA_CONNECTED:
			 	ESP_LOGI(TAG, "WIFI_EVENT_STA_CONNECTED");
			 	break;
				
			 case WIFI_EVENT_STA_DISCONNECTED:
			 	ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED");
			 	wifiApp_sta_disconnectedLogInfo(eventData_p);
    
			    if( g_retry_number < MAX_CONNECTION_RETRIES)
			    {
					esp_wifi_connect();
					g_retry_number++;
				}
			    else
				{
					wifiApp_sendMessage(WIFI_APP_STA_DISCONNECTED);
				}
			 	break;
		 }
	 }
	 else if (event_base == IP_EVENT)
	 {
		 switch(eventId)
		 {
			 case IP_EVENT_STA_GOT_IP:
			 	ESP_LOGI(TAG, "IP_EVENT_STA_GOT_IP");
			 	
			 	wifiApp_sendMessage(WIFI_APP_STA_CONNECTED_GOT_IP);
			 	
			 	break;
		 }
	 }
 }

/**
 * Initializes the WiFi application event handler for WiFi and IP events.
 */
static void wifiApp_eventHandler_init(void)
{
	// Event loop for the WiFi driver
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	
	// Event handler for the connection
	esp_event_handler_instance_t instance_wifi_event;
	esp_event_handler_instance_t instance_ip_event;
	ESP_ERROR_CHECK(esp_event_handler_register( WIFI_EVENT,
												ESP_EVENT_ANY_ID,
												&wifiApp_event_handler,
												&instance_wifi_event));
	ESP_ERROR_CHECK(esp_event_handler_register( IP_EVENT,
												ESP_EVENT_ANY_ID,
												&wifiApp_event_handler,
												&instance_ip_event));
}


/**
 * Initializes TCP stack and default WiFi configuration.
 */
static void wifiApp_defaultWifi_init(void)
{
	// Initializes the TCP stack
	ESP_ERROR_CHECK(esp_netif_init());
	
	// Default Wifi config - operations must be in this order!
	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	esp_netif_sta = esp_netif_create_default_wifi_sta();
	esp_netif_ap = esp_netif_create_default_wifi_ap();
}


/**
 * Configures the WiFi access point settings and assigns the static IP to the SoftAP.
 */
static void wifiApp_softAP_config(void)
{
	// SoftAP - WiFi access point configuration
	wifi_config_t ap_config =
	{
		.ap = {
			.ssid 			= 	WIFI_AP_SSID,
			.ssid_len 		=	strlen(WIFI_AP_SSID),
			.password 		=	WIFI_AP_PASSWORD,
			.channel 		=	WIFI_AP_CHANNEL,
			.ssid_hidden	=	WIFI_AP_SSID_HIDDEN,
			.authmode		=	WIFI_AUTH_WPA2_PSK,
			.max_connection	=	WIFI_AP_MAX_CONNECTIONS,
			.beacon_interval=	WIFI_AP_BEACON_INTERVAL,
		},
	};
	
	// Configure DHCP for the AP
	esp_netif_ip_info_t ap_ip_info;
	memset(&ap_ip_info, 0x00, sizeof(ap_ip_info));
	
	// Stopping the DHCP server before updating it
	esp_netif_dhcps_stop(esp_netif_ap);
	
	// Assign access point's static IP, GW and Netmask
	//(converts string, to the wanted binary form)
	inet_pton(AF_INET, WIFI_AP_IP, &ap_ip_info.ip);
	inet_pton(AF_INET, WIFI_AP_GATEWAY, &ap_ip_info.gw);
	inet_pton(AF_INET, WIFI_AP_NETMASK, &ap_ip_info.netmask);
	
	// Statically configure the network interface
	ESP_ERROR_CHECK(esp_netif_set_ip_info(esp_netif_ap, &ap_ip_info));
	// Start the AP DHCP server (for connecting stations e.g. your mobile device)
	ESP_ERROR_CHECK(esp_netif_dhcps_start(esp_netif_ap));
	
	// Setting the mode as Access Point / Station Mode
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config));	///> set configuration
	ESP_ERROR_CHECK(esp_wifi_set_bandwidth(WIFI_IF_AP, WIFI_AP_BANDWIDTH));	///> default bandwidth 20 MHz
	ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_STA_POWER_SAVE));						///> power save set to "none"
}

/**
 * Connects the ESP32 to an external AP using the updated station configuration
 */
static void wifiApp_sta_connect(void)
{
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, wifiApp_getWifiConfig()));
	ESP_ERROR_CHECK(esp_wifi_connect());
}

/**
 * A function to log at the serial monitor the reasons of why the station disconnected
 */
static void wifiApp_sta_disconnectedLogInfo(void * eventData_p)
{
	wifi_event_sta_disconnected_t *wifi_event = (wifi_event_sta_disconnected_t *)eventData_p;
    ESP_LOGI(TAG, "WiFi disconnected, reason: %d", wifi_event->reason);
}