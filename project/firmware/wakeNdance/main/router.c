/*
 * router.c
 *
 *  Created on: 17 de nov. de 2024
 *      Author: Luiz Carlos
 */


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries
#include <stdio.h>
#include <string.h>

// ESP libraries
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "cJSON.h"

// Personal libraries
#include "esp_netif.h"
#include "esp_netif_ip_addr.h"
#include "esp_netif_types.h"
#include "esp_wifi.h"
#include "esp_wifi_types_generic.h"
#include "httpServer.h"
#include "wifiApp.h"
#include "router.h"



/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "http_app_server";

// Netif object for the Station and Access Point
extern esp_netif_t * esp_netif_sta;
extern esp_netif_t * esp_netif_ap;

// Buffer for LocalTime json string
char localJSONObjBuffer[BUFFER_MAX_SIZE] = {0};
char localTimeJSON[BUFFER_MAX_SIZE] = {0};


/* Static Functions */

//App functions
//static void router_wifiApp_connectedEvents(void);

// URI handler functions
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_connect_json)(httpd_req_t *req);
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_connect_status_json)(httpd_req_t *req);
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(get_wifi_connect_info_json)(httpd_req_t *req);
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(wifi_disconnect_json)(httpd_req_t *req);
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(get_local_time_json)(httpd_req_t *req);
static void router_uri_register(void);



/**************************
**	   APP FUNCTIONS 	 **
**************************/

void router_setup(void)
{
	// Allocate the api routes inside the httpServer code
	httpServer_setApiRoutes(&router_uri_register);
	
	// Set the wifi connected event callback function
//	wifiApp_setCallback(router_wifiApp_connectedEvents);
	
	// Start WiFi
	wifiApp_start();
	
	// Clean localTimeJSON buffer
	memset(localTimeJSON, 0, BUFFER_MAX_SIZE);
	
	
}

//static void router_wifiApp_connectedEvents(void)
//{
//	ESP_LOGI(TAG, "WiFi Application Connected!");
//	sntp_clock_init();
//}



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
	// char localJSONObjBuffer[BUFFER_MAX_SIZE];
	size_t lenBodyJson = 0;
	size_t lenPass;
	size_t lenSSID;

	char * ssid_p;
	char * pwd_p;
	wifi_config_t * wifi_config_p = NULL;
	
	ESP_LOGI(TAG, "/wifiConnect.json requested");
	
	memset(localJSONObjBuffer,0, BUFFER_MAX_SIZE);

	// Get Request Body
	lenBodyJson = req->content_len;
	httpd_req_recv(req, localJSONObjBuffer, lenBodyJson);
	printf("Data sent by the client: %.*s\n",lenBodyJson, localJSONObjBuffer);

	// Parse the JSON data (example using cJSON)
	cJSON *body_json = cJSON_Parse(localJSONObjBuffer);
	if (!body_json) {
		ESP_LOGE(TAG, "Failed to parse JSON data");
		cJSON_Delete(body_json);
		return ESP_FAIL;
	}

	cJSON *ssid_json = cJSON_GetObjectItemCaseSensitive(body_json, "c_ssid");
	cJSON *pwd_json = cJSON_GetObjectItemCaseSensitive(body_json, "c_pwd");

	if (!ssid_json || !pwd_json) {
		ESP_LOGE(TAG, "Missing 'c_ssid' or 'c_pwd' in JSON data");
		cJSON_Delete(body_json);
		return ESP_FAIL;
	}

	if (!cJSON_IsString(ssid_json) || !cJSON_IsString(pwd_json)) {
		ESP_LOGE(TAG, "Invalid data type for 'c_ssid' or 'c_pwd'");
		cJSON_Delete(body_json);
		return ESP_FAIL;
	}


	// Get SSID from body
	// if (lenSSID > 1)
	// {
		ssid_p = wifiApp_getStationSSID();
		memset(ssid_p, 0x00, WIFI_SSID_LENGTH);
		ssid_p = ssid_json->valuestring;
		lenSSID = strlen(ssid_p);
	// // }
	
	// Get Password from body
	// if (lenPass > 1)
	// {
		pwd_p = wifiApp_getStationPassword();
		memset(pwd_p, 0x00, WIFI_PASSWORD_LENGTH);
		pwd_p = pwd_json->valuestring;
		lenPass = strlen(pwd_p);
	// }
	
	// Update the WiFi networks configuration and let the WiFi applications know
	wifi_config_p = wifiApp_getWifiConfig();
	memset(wifi_config_p, 0x00, sizeof(wifi_config_t));
	memcpy(wifi_config_p->sta.ssid, ssid_p, lenSSID);
	memcpy(wifi_config_p->sta.password, pwd_p, lenPass);
	wifiApp_sendMessage(WIFI_APP_CONNECTING_FROM_HTTP_SERVER);

	cJSON_Delete(body_json);
	
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
	
	memset(localJSONObjBuffer, 0, BUFFER_MAX_SIZE);
	
	sprintf(localJSONObjBuffer, "{\"wifi_connect_status_json\":%d}", *httpServer_get_wifiConnectStatus());
	httpd_resp_set_type(req, "application/json");
	httpd_resp_send(req, localJSONObjBuffer, strlen(localJSONObjBuffer));
	
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

	if (*httpServer_get_wifiConnectStatus() == HTTP_WIFI_STATUS_CONNECT_SUCCESS)
	{
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
 * localTime.json handler responds by sending the local time.
 * @param req HTTP request for which the uri needs to be handled.
 * @return ESP_OK
 */
static esp_err_t APP_URI_FUNCTION_HANDLER_NAME(get_local_time_json)(httpd_req_t *req)
{
//	ESP_LOGI(TAG, "/localTime.json requested");
//	
//	if(strlen(localTimeJSON) > 0)
//	{
//		sprintf(localTimeJSON, "{\"time\":\"%s\"}", sntp_clock_getTime());
//	}
//	
//	httpd_resp_set_type(req, "application/json");
//	httpd_resp_send(req, localTimeJSON, strlen(localTimeJSON));
	
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