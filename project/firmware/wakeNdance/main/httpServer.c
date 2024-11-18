/*
 * httpServer.c
 *
 *  Created on: 17 de nov. de 2024
 *      Author: Luiz Carlos
 */

/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries

// ESP libraries
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"

// Personal libraries
#include "httpServer.h"
#include "tasks_common.h"



/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "http_server";

// WiFi connect status
static http_server_wifi_connect_status_e g_wifi_connect_status = NONE;

// HTTP server task handle
static httpd_handle_t http_server_handle = NULL; ///> used on start and stop server

// Embedded files: JQuery, index.html, app.css, app.js and favicon.ico files
#define X(uri_handler, file, http_resp_type, start, end) \
	extern const uint8_t uri_handler##_start[]	asm(#start); \
	extern const uint8_t uri_handler##_end[]	asm(#end);
	X_MACRO_HTTP_SERVER_URI_HANDLER_LIST
#undef X
	
	/* Function Pointers */
void (* httpServer_uri_setApiRoutes_fp)(void);


	/* FreeRTOS Structures */

// HTTP server monitor task handle
static TaskHandle_t task_http_server_monitor = NULL;

// Queue handle used to manipulate the main queue of events
static QueueHandle_t http_server_monitor_queue_handle;



/**************************
** UPPERLAYER FUNCTIONS	 **
**************************/

/**
 * Function to get routers from another file to be declared here.
 * Separating the include files routes, from the api ones.
 * @param apiFunction a function from an upper layer, where other
 * uri routes are declared with the httpServer_uri_registerHandler function.
 */
void httpServer_setApiRoutes(void (*apiFunction)(void))
{
	httpServer_uri_setApiRoutes_fp = apiFunction;
}

/**
 * Function to register an HTTP uri.
 * @param route the http route on the IP server
 * @param method if the route is accessed by GET, POST, PUT...
 * @param handler the function handler that specifies what happens when the route is accessed.
 */
void httpServer_uri_registerHandler(const char* route, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *req))
{
	httpd_uri_t uri_handler = {
		.uri 		= route,
		.method 	= method,
		.handler	= handler,
		.user_ctx	= NULL,
	};
	httpd_register_uri_handler(http_server_handle, &(uri_handler));
}


/**************************
**	FreeRTOS FUNCTIONS	 **
**************************/

/**
 * HTTP server monitor task used to track events of the HTTP server
 * @param pvParameters parameter which can be passed to the task.
 */
static void httpServer_freeRTOS_monitor(void * parameter)
{
	http_server_queue_message_t msg;
	
	for(;;)
	{
		if(xQueueReceive(http_server_monitor_queue_handle, &msg, portMAX_DELAY))
		{
			switch (msg.msgId)
			{
				case HTTP_WIFI_CONNECT_INIT:
					ESP_LOGI(TAG, "HTTP_WIFI_CONNECT_INIT");
					
					g_wifi_connect_status = HTTP_WIFI_STATUS_CONNECTING;
					
					break;
					
				case HTTP_WIFI_CONNECT_SUCCESS:
					ESP_LOGI(TAG, "HTTP_WIFI_CONNECT_SUCCESS");
					
					g_wifi_connect_status = HTTP_WIFI_STATUS_CONNECT_SUCCESS;
					
					break;
					
				case HTTP_WIFI_CONNECT_FAIL:
					ESP_LOGI(TAG, "HTTP_WIFI_CONNECT_FAIL");
					
					g_wifi_connect_status = HTTP_WIFI_STATUS_CONNECT_FAILED;
					break;
					
//				case HTTP_OTA_UPDATE_SUCCESSFULL:
//					ESP_LOGI(TAG, "HTTP_OTA_UPDATE_SUCCESSFULL");
//					break;
//					
//				case HTTP_OTA_UPDATE_FAILED:
//					ESP_LOGI(TAG, "HTTP_OTA_UPDATE_FAILED");
//					break;
					
				default:
					break;
			}
		}
	}
}

/**
 * Setup the FreeRTOS environment for HTTP server.
 */
static void httpServer_freeRTOS_setup(void)
{
	// Create HTTP server monitor task
	xTaskCreatePinnedToCore(&httpServer_freeRTOS_monitor,
							"httpServer_monitor",
							HTTP_SERVER_MONITOR_STACK_SIZE,
							NULL,
							HTTP_SERVER_MONITOR_PRIORITY,
							&task_http_server_monitor,
							HTTP_SERVER_MONITOR_CORE_ID);
	
	// Create the message queue
	http_server_monitor_queue_handle = xQueueCreate(3, sizeof(http_server_queue_message_t));
}

static void httpServer_freeRTOS_endTask(void)
{
	if(task_http_server_monitor)
	{
		vTaskDelete(task_http_server_monitor);
		ESP_LOGI(TAG, "httpServer_freeRTOS_endTask: stopping HTTP server monitor");
		task_http_server_monitor = NULL;
	}
}

/**
 * Sends a message to the queue
 * @param msgId message ID from the http_server_state_e enum.
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE.
 */
BaseType_t httpServer_monitor_sendMessage(http_server_state_e msgId)
{
	http_server_queue_message_t msg;
	msg.msgId = msgId;
	return xQueueSend(http_server_monitor_queue_handle, &msg, portMAX_DELAY);
}



/**************************
**	   APP FUNCTIONS	 **
**************************/

/**
 * Functions that get uri handler for when the files are requested when accessing the web page.
 * @param req HTTP request for which the uri needs to be handled.
 * @return ESP_OK
 */
#define X(uri_handler, file, http_resp_type, start, end) \
	static esp_err_t URI_FUNCTION_HANDLER_NAME(uri_handler)(httpd_req_t *req)	\
	{ 																			\
		ESP_LOGI(TAG, "%s requested", #uri_handler);							\
		\
		httpd_resp_set_type(req, http_resp_type);								\
		httpd_resp_send(req, (const char *)uri_handler##_start, uri_handler##_end - uri_handler##_start);\
		\
		return ESP_OK;\
	}
	X_MACRO_HTTP_SERVER_URI_HANDLER_LIST
	
#undef X
 
/**
 * Sets up the default http server configuration
 * @param httpd_config_t the httpd config structure for the HTTP server task
 */
static void httpServer_configure(httpd_config_t * config)
{	 
//	 *config = HTTPD_DEFAULT_CONFIG();
	
	// The core that the HTTP server will run on
	config->core_id = HTTP_SERVER_TASK_CORE_ID;
	
	// Adjust the default priority to 1 less than the WiFi application task
	config->task_priority = HTTP_SERVER_TASK_PRIORITY;
	
	// Bump up the stack size (default is 4096)
	config->stack_size = HTTP_SERVER_STACK_SIZE;
	
	// Increase uri handlers
	config->max_uri_handlers = HTTP_SERVER_MAX_URI_HANDLERS;
	
	// Increase the timeout limits
	config->recv_wait_timeout = HTTP_SERVER_TIMEOUT_LIMIT;
	config->send_wait_timeout = HTTP_SERVER_TIMEOUT_LIMIT;
	
	ESP_LOGI(TAG, "httpServer_configure: Starting server on port: '%d' with task priority: '%d'",
					config->server_port,
					config->task_priority);
}

/**
 * Registers the URI handlers
 */
static void httpServer_uri_setFilesHandlersAndRoutes(void)
{
	#define X(uri_handler, file, http_resp_type, start, end)	\
	httpServer_uri_registerHandler(file, HTTP_GET,URI_FUNCTION_HANDLER_NAME(uri_handler));
	
		X_MACRO_HTTP_SERVER_URI_HANDLER_LIST
		
	#undef X
}

static void httpServer_uri_setRoutesFromOtherFiles(void)
{
	if (httpServer_uri_setApiRoutes_fp)
	{
		httpServer_uri_setApiRoutes_fp();
	}
}
 
 /**
  * Starts the HTTP server.
  */
void httpServer_start(void)
{
	if (http_server_handle == NULL)
	{
		// Initializing and configurating the structure
		httpd_config_t config = HTTPD_DEFAULT_CONFIG();
		httpServer_configure(&config);
		
		// Create HTTP server monitor task and the message queue
		httpServer_freeRTOS_setup();
		
		//Start the httpd server
		if (httpd_start(&http_server_handle, &config) == ESP_OK)
		{
			ESP_LOGI(TAG, "httpServer_configure: Registering URI handlers");
			httpServer_uri_setFilesHandlersAndRoutes();
			httpServer_uri_setRoutesFromOtherFiles();
		} else
		{
			http_server_handle = NULL;
		}
	}
}

 /**
  * Stops the HTTP server.
  */
void httpServer_stop(void)
{
	if (http_server_handle)
	{
		httpd_stop(http_server_handle);
		ESP_LOGI(TAG, "httpServer_stop: stopping HTTP server");
		http_server_handle = NULL;
	}
	// Stop monitor task
	httpServer_freeRTOS_endTask();
}
