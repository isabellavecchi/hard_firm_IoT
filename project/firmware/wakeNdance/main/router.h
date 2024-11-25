/*
 * router.h
 *
 *  Created on: 17 de nov. de 2024
 *      Author: Luiz Carlos
 */

#ifndef MAIN_ROUTER_H_
#define MAIN_ROUTER_H_


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries
#include <string.h>

// ESP libraries
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"

// Personal libraries
#include "esp_wifi_types_generic.h"
#include "wifiApp.h"
#include "httpServer.h"
#include "sntp.h"


/**************************
**		DEFINITIONS		 **
**************************/

#define APP_URI_FUNCTION_HANDLER_NAME(uri) webRouter_##uri##_handler 

#define X_MACRO_API_ROUTES_LIST \
	X(0, wifi_connect_json, 			"/wifiConnect.json",		HTTP_POST, 		"application/json") \
	X(1, wifi_connect_status_json,		"/wifiConnectStatus",		HTTP_POST, 		"application/json") \
	X(2, get_wifi_connect_info_json,	"/wifiConnectInfo.json",	HTTP_GET,		"application/json") \
	X(3, wifi_disconnect_json,			"/wifiDisconnect.json",		HTTP_DELETE,	"application/json") \
	X(4, get_local_time_json,			"/localTime.json",			HTTP_GET,		"application/json")


/**************************
**		FUNCTIONS		 **
**************************/

void router_setup(void);

#endif /* MAIN_ROUTER_H_ */
