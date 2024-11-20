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

// ESP libraries
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/apps/sntp.h"

// Personal libraries
#include "portmacro.h"
#include "tasks_common.h"
#include "httpServer.h"
#include "wifiApp.h"
#include "sntp_timeSync.h"
#include <time.h>



/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "nstp_tymeSync";

// Buffer to allocate the current time string
static char timeBuffer[100];


	/* FreeRTOS Structure */



/**************************
**		FUNCTIONS		 **
**************************/

static uint8_t sntp_timeSync_isTimeDeprecated(struct tm * timeInfo)
{
	time_t now = 0;
	time(&now);
	localtime_r(&now, timeInfo);
	
	// Check the time, in case we need to initialize/reinitialize
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
	
	// Set server name from where to pull timestamp data from
	sntp_setservername(0, "pool.ntp.org");
	
	// Initialize the servers
	sntp_init();
	
	// Let the http_server know service is initialized
	httpServer_monitor_sendMessage(HTTP_TIME_SERVICE_INITIALIZED);
	
	// Set the local time zone
	setenv("TZ", "America/Sao_Paulo", 1);
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