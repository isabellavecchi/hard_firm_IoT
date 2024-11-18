/*
 * router.c
 *
 *  Created on: 17 de nov. de 2024
 *      Author: Luiz Carlos
 */


/**************************
**		  INCLUDES	 	 **
**************************/

// ESP libraries
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"

// Personal libraries
#include "esp_wifi_types_generic.h"
#include "webRouter.h"
#include "wifiApp.h"
#include "httpServer.h"
#include <string.h>



/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "http_app_server";


	/* Static Functions */
	
//HANDLER FUNCTIONS
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_connect_json)(httpd_req_t *req);
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_connect_status_json)(httpd_req_t *req);


/**************************
**	 HANDLER FUNCTIONS 	 **
**************************/

/**
 * wifiConnect.json handler is invoked after the connect button is pressed
 * and handles receiving the SSID and password entered by the user
 * @param req HTTP request for which the uri needs to be handled.
 * @return ESP_OK
 */
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_connect_json)(httpd_req_t *req)
{	
	size_t lenSSID = 0, lenPass = 0;
	char * ssid_p = NULL, * pwd_p = NULL;
	wifi_config_t * wifi_config_p = NULL;
	
	ESP_LOGI(TAG, "/wifiConnect.json requested");
	
	// Get SSID header
	lenSSID = httpd_req_get_hdr_value_len(req, "my-connect-ssid") + 1;
	if (lenSSID > 1)
	{
		ssid_p = wifiApp_getStationSSID();
		memset(ssid_p, 0x00, WIFI_SSID_LENGTH);
		if(httpd_req_get_hdr_value_str(req, "my-connect-ssid",ssid_p, lenSSID))
		{
			ESP_LOGI(TAG, "webRouter_wifi_connect_json_handler: Found header => my-connect-ssid: %s", ssid_p);
		}
	}
	
	// Get Password header
	lenPass = httpd_req_get_hdr_value_len(req, "my-connect-pwd") + 1;
	if (lenPass > 1)
	{
		pwd_p = wifiApp_getStationPassword();
		memset(pwd_p, 0x00, WIFI_PASSWORD_LENGTH);
		if(httpd_req_get_hdr_value_str(req, "my-connect-pwd",pwd_p, lenPass))
		{
			ESP_LOGI(TAG, "webRouter_wifi_connect_json_handler: Found header => my-connect-pwd: %s", pwd_p);
		}
	}
	
	// Update the WiFi networks configuration and let the WiFi applications know
	wifi_config_p = wifiApp_getWifiConfig();
	memset(wifi_config_p, 0x00, sizeof(wifi_config_t));
	memcpy(wifi_config_p->sta.ssid, ssid_p, lenSSID);
	memcpy(wifi_config_p->sta.password, pwd_p, lenPass);
	wifiApp_sendMessage(WIFI_APP_CONNECTING_FROM_HTTP_SERVER);
	
	return ESP_OK;
}



/**
 * wifiConnect.json handler is invoked after the connect button is pressed
 * and handles receiving the SSID and password entered by the user
 * @param req HTTP request for which the uri needs to be handled.
 * @return ESP_OK
 */
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_connect_status_json)(httpd_req_t *req)
{	
	return ESP_OK;
}


static void webRouter_uri_register(void)
{
	#define X(handler, route, method) \
		httpServer_uri_registerHandler(route, method, APP_URI_FUNCTION_HANDLER_NAME(handler));
		X_MACRO_API_ROUTES_LIST
	#undef X
}











