/*
 * router.h
 *
 *  Created on: 17 de nov. de 2024
 *      Author: Luiz Carlos
 */

#ifndef MAIN_WEBROUTER_H_
#define MAIN_WEBROUTER_H_


/**************************
**		  INCLUDES	 	 **
**************************/

// ESP libraries
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"

// Personal libraries
#include "esp_wifi_types_generic.h"
#include "wifiApp.h"
#include "httpServer.h"
#include <string.h>


/**************************
**		DEFINITIONS		 **
**************************/

#define APP_URI_FUNCTION_HANDLER_NAME(uri) webRouter_##uri##_handler 

#define X_MACRO_API_ROUTES_LIST \
	X(wifi_connect_json, 		"/wifiConnect.json",	HTTP_POST) \
	X(wifi_connect_status_json,	"/wifiConnectStatus",	HTTP_POST)


#endif /* MAIN_WEBROUTER_H_ */
