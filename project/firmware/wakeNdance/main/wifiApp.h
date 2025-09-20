/*
 * wifiApp.h
 *
 *  Created on: 16 de nov. de 2024
 *      Author: Luiz Carlos
 */

#ifndef MAIN_WIFIAPP_H_
#define MAIN_WIFIAPP_H_


/**************************
**		  INCLUDES	 	 **
**************************/

// ESP libraries
#include "esp_netif.h"
#include "esp_wifi_types_generic.h"
#include "freertos/portmacro.h"

// Personal libraries
#include "projectConfig.h"


/**************************
**		DEFINITIONS		 **
**************************/


// Macro to define the function name of each state
#define WIFI_STATE_FUNC_NAME(state) wifiApp_sm_ ## state ## _fn

/**
 * List used to create all the structures for the WiFi state machine
 * based on X definition
*/
#define X_MACRO_WIFI_STATE_LIST 				\
	X(0, WIFI_APP_START_HTTP_SERVER				) \
	X(1, WIFI_APP_CONNECTING_FROM_HTTP_SERVER	) \
	X(2, WIFI_APP_STA_CONNECTED_GOT_IP			) \
	X(3, WIFI_APP_USER_REQUESTED_STA_DISCONNECT	) \
	X(4, WIFI_APP_STA_DISCONNECTED				) 



/**************************
**		STRUCTURES		 **
**************************/

/**
 * Wifi callback function type
 */
typedef void(*wifi_connected_event_callback_t)(void);

/**
 * Message IDs for the WiFi application task
 */
typedef enum
{
	#define X(ID, ENUM) ENUM=ID,
		X_MACRO_WIFI_STATE_LIST
	#undef X
} sm_wifi_app_state_e;

/**
 * Messages for each WiFi application task State
 */
static const char * sm_wifi_app_state_names[] = 
{
#define X(ID, ENUM) #ENUM,
	X_MACRO_WIFI_STATE_LIST
#undef X
};

/**
 * Structure for the message queue
 */
typedef struct wifi_app_queue_message_s
{
    sm_wifi_app_state_e  wifiApp_state;
} wifi_app_queue_message_t;

// Defining a type for every function of the WiFi state machine
typedef void (*sm_wifi_app_function)(wifi_app_queue_message_t * st);

// Creating a struct to hold those functions later on an array
typedef struct sm_wifi_table_fn_s
{
	sm_wifi_app_function func;
} sm_wifi_table_fn_t;



/**************************
**		  GETTERS		 **
**************************/

/**
 * Returns the Address of the first char of SSID string
 */
char * wifiApp_getStationSSID(void);

/**
 * Returns the Address of the first char of password string
 */
char * wifiApp_getStationPassword(void);

/**
 * Gets the WiFi configuration
 */
 wifi_config_t * wifiApp_getWifiConfig(void);



/**************************
**		FUNCTIONS		 **
**************************/

/**
 * Sends a message to the queue
 * @param msgId message ID from the wifi_app_message_e enum
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE
 */
BaseType_t wifiApp_sendMessage(sm_wifi_app_state_e msgId);

/**
 * Starts the WiFi RTOS task
 */
void wifiApp_start(void);

/**
* Sets the callback function
*/
void wifiApp_setCallback(wifi_connected_event_callback_t cbFunction);


#endif /* MAIN_WIFIAPP_H_ */
