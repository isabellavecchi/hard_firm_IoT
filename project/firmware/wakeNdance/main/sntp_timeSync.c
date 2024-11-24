/*
 * nstp_tymeSync.c
 *
 *  Created on: 20 de nov. de 2024
 *      Author: IVecchiFerreira
 */

/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries
#include <string.h>
#include <time.h>
#include <sys/time.h>

// ESP libraries
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_netif_sntp.h"
#include "lwip/ip_addr.h"
#include "esp_sntp.h"

// Personal libraries
#include "tasks_common.h"
#include "httpServer.h"
#include "wifiApp.h"
#include "sntp_timeSync.h"



/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "nstp_tymeSync";

// Buffer to allocate the current time string
static char timeBuffer[100];

// Flag that inform if local time is settled or not
static uint8_t fg_isLocalTimeSet = 0;


	/* FreeRTOS Structure */


	/* Static Functions */
static void sntp_timeSync_setInterval(struct timeval *tv);
static uint8_t sntp_timeSync_isTimeDeprecated(struct tm * timeInfo);
static void sntp_timeSync_obtainTime(void);
static void sntp_timeSync_task(void * pvParam);


/**************************
**		FUNCTIONS		 **
**************************/

/**
 * Function to customize time sync interval.
 * @param timeval *tv struct with the interval settings.
 */
static void sntp_timeSync_setInterval(struct timeval *tv)
{
   settimeofday(tv, NULL);
   ESP_LOGI(TAG, "Time is synchronized from custom code");
   sntp_set_sync_status(SNTP_SYNC_STATUS_COMPLETED);
}

/**
 * Function that notificates a time synchronization event.
 */
void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

static uint8_t sntp_timeSync_isTimeDeprecated(struct tm * timeInfo)
{
	time_t now = 0;
	time(&now);
	localtime_r(&now, timeInfo);
	
	// Check the time, in case we need to initialize/reinitialize
    // Is time set? If not, tm_year will be (1970 - 1900).
	if(timeInfo->tm_year < (2016 - 1900))
	{
		ESP_LOGI(TAG, "Time is not set yet");
		return 1;
	}
	return 0;
}

/**
 * Gets the current time, and if the current time is not up to date,
 * the sntp_timeSync_init function is called.
 * @param arg pvParam.
 */
static void sntp_timeSync_obtainTime(void)
{	
	struct tm timeInfo = {0};
	
	// Check the time, in case we need to initialize/reinitialize
	if(sntp_timeSync_isTimeDeprecated(&timeInfo))
	{
		sntp_timeSync_init();
	}
}

/**
 * The SNTP time synchronization task.
 * @param arg pvParam.
 */
static void sntp_timeSync_task(void * pvParam)
{
	while(1)
	{
		sntp_timeSync_obtainTime();
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
	// If the task ever break out of the loop, we delete it
	vTaskDelete(NULL);
}

/**
 * Setup the local timezone and operating mode
 */
void sntp_timeSync_setup(void)
{
	ESP_LOGI(TAG, "Setting up the SNTP service");
	
	// Set the operating mode
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
}

/**
 * Setup the local timezone and operating mode
 */
void sntp_timeSync_init(void)
{
	ESP_LOGI(TAG, "Initializing the SNTP service");
	
	esp_sntp_config_t sntp_config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
	esp_netif_sntp_init(&sntp_config);
	
//	// Let the http_server know service is initialized
//	httpServer_monitor_sendMessage(HTTP_TIME_SERVICE_INITIALIZED);

	
	// Set the local time zone
	setenv("TZ", "BRST+3BRDT+2,M10.3.0,M2.3.0", 1);
	tzset();
}

/**
 * Returns local time if set.
 * @return local time buffer.
 */
char* sntp_timeSync_getTime(void)
{
	memset(timeBuffer, 0, 100);
	
	struct tm timeInfo = {0};
	
	// Check if the time, is valid
	if(!sntp_timeSync_isTimeDeprecated(&timeInfo))
	{
		strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M", &timeInfo);
		ESP_LOGI(TAG, "Current time info: %s", timeBuffer);
	}
	
	return timeBuffer;
}

/**
 * A function that informs if the localtime is informed or not.
 */
uint8_t router_isLocalTimeSet(void)
{
	return fg_isLocalTimeSet;
}

/**
 * Starts the SNTP server synchronization task.
 */
void sntp_timeSync_task_start(void)
{
	xTaskCreatePinnedToCore(&sntp_timeSync_task,
							"sntp_time_sync",
							SNTP_TIME_SYNC_TASK_STACK_SIZE,
							NULL,
							SNTP_TIME_SYNC_TASK_PRIORITY,
							NULL,
							SNTP_TIME_SYNC_TASK_CORE_ID);
	
}