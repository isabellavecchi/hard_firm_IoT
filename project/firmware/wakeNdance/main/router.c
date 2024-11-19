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
#include "esp_netif.h"
#include "esp_netif_ip_addr.h"
#include "esp_netif_types.h"
#include "esp_wifi.h"
#include "esp_wifi_types_generic.h"
#include "httpServer.h"
#include "wifiApp.h"
#include "router.h"
#include <stdio.h>
#include <string.h>



/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "http_app_server";

// Netif object for the Station and Access Point
extern esp_netif_t * esp_netif_sta;
extern esp_netif_t * esp_netif_ap;




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
			ESP_LOGI(TAG, "router_wifi_connect_json_handler: Found header => my-connect-ssid: %s", ssid_p);
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
			ESP_LOGI(TAG, "router_wifi_connect_json_handler: Found header => my-connect-pwd: %s", pwd_p);
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
 * wifiConnectStatus handler updates the connection status for the web page.
 * and handles receiving the SSID and password entered by the user
 * @param req HTTP request for which the uri needs to be handled.
 * @return ESP_OK
 */
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_connect_status_json)(httpd_req_t *req)
{	
	ESP_LOGI(TAG, "/wifiConnectStatus requested");
	
	char statusJSON[100];
	
	sprintf(statusJSON, "{\"wifi_connect_status_json\":%d}", *httpServer_get_wifiConnectStatus());
	httpd_resp_set_type(req, "application/json");
	httpd_resp_send(req, statusJSON, strlen(statusJSON));
	
	return ESP_OK;
}

/**
 * wifiConnectInfo.json handler updates the web page with connection information.
 * and handles receiving the SSID and password entered by the user
 * @param req HTTP request for which the uri needs to be handled.
 * @return ESP_OK
 */
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(get_wifi_connect_info_json)(httpd_req_t *req)
{
	ESP_LOGI(TAG, "/wifiConnectInfo.json requested");
	
	char ipInfoJSON[200];
	memset(ipInfoJSON, 0x00, sizeof(ipInfoJSON));
	
	char ip[IP4ADDR_STRLEN_MAX];
	char netmask[IP4ADDR_STRLEN_MAX];
	char gateway[IP4ADDR_STRLEN_MAX];

	ESP_LOGI(TAG, "EH PRA MOSTRAR OS TREM DA INTERNET AQUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
	if (*httpServer_get_wifiConnectStatus() == HTTP_WIFI_STATUS_CONNECT_SUCCESS)
	{
		ESP_LOGI(TAG, "ERA PRA TER MOSTRADO OS TREM DA INTERNET AQUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
		wifi_ap_record_t wifi_data;
		ESP_ERROR_CHECK(esp_wifi_sta_get_ap_info(&wifi_data));
		char * ssid = (char*)wifi_data.ssid;
		
		esp_netif_ip_info_t ip_info;
		ESP_ERROR_CHECK(esp_netif_get_ip_info(esp_netif_sta, &ip_info));
		esp_ip4addr_ntoa(&ip_info.ip, ip, IP4ADDR_STRLEN_MAX);
		esp_ip4addr_ntoa(&ip_info.netmask, netmask, IP4ADDR_STRLEN_MAX);
		esp_ip4addr_ntoa(&ip_info.gw, gateway, IP4ADDR_STRLEN_MAX);
		
		sprintf(ipInfoJSON, "{\"ip\":\"%s\",\"netmask\":\"%s\",\"gateway\":\"%s\",\"ap\":\"%s\"}", ip, netmask, gateway, ssid);
	}
	
	httpd_resp_set_type(req, "application/json");
	httpd_resp_send(req, ipInfoJSON, strlen(ipInfoJSON));
	
	return ESP_OK;
}

/**
 * wifiDisconnect.json handler responds by sending a message to WiFi application to disconnect.
 * and handles receiving the SSID and password entered by the user
 * @param req HTTP request for which the uri needs to be handled.
 * @return ESP_OK
 */
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_disconnect_json)(httpd_req_t *req)
{
	ESP_LOGI(TAG, "/wifiDisconnect.json requested");
	
	wifiApp_sendMessage(WIFI_APP_USER_REQUESTED_STA_DISCONNECT);
	
	return ESP_OK;
}

/**
 * A function that will make the uri's available to the server.
 */
static void router_uri_register(void)
{
	#define X(id, handler, route, method, ansType) \
		httpServer_uri_registerHandler(route, method, APP_URI_FUNCTION_HANDLER_NAME(handler));
		X_MACRO_API_ROUTES_LIST
	#undef X
}





void router_setup(void)
{
	// Allocate the api routes inside the httpServer code
	httpServer_setApiRoutes(&router_uri_register);
	
	// Start WiFi
	wifiApp_start();
}








