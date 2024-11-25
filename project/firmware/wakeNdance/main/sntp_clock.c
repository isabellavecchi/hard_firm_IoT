///*
//* sntp_clock.c
//*
//*  Created on: 23 de nov. de 2024
//*      Author: IVecchiFerreira
//*/
//
///**************************
//**		  INCLUDES	 	 **
//**************************/
//
//// C libraries
//#include <string.h>
//#include <time.h>
//#include <sys/time.h>
//
//// ESP libraries
//#include "esp_log.h"
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "lwip/ip_addr.h"
//#include "esp_netif_sntp.h"
//#include "esp_sntp.h"
//
//// Personal libraries
//#include "tasks_common.h"
//#include "httpServer.h"
//#include "wifiApp.h"
//#include "sntp_clock.h"
//
//
///**************************
//**		DECLARATIONS	 **
//**************************/
//
//	/* Variables */
//
//// Tag used for ESP serial console messages
//static const char TAG[] = "sntp_clock";
//
//// Buffer to allocate the current time string
//static char timeBuffer[100];
//
//// Structures for this file
//static sntp_clock_status_e sntp_clock_status = CLOCK_SNTP_CONNECTION_NOT_FOUND;
//static sntp_clock_t sntp_clock_v;
//
//	/* Static Functions */
//
//// Static Functions
//static void sntp_timeSync_obtainTime(void);
//static void sntp_timeSync_setLocalTimeZone(void);
//static uint8_t sntp_timeSync_isTimeDeprecated(struct tm * timeInfo);
//static void sntp_timeSync_task(void *pvParam);
//static void sntp_task_init(void);
//
//
//
///**************************
//**		FUNCTIONS		 **
//**************************/
//
///**
//* Function that sets up the SNTP time sync
//*/
//void sntp_clock_setup(void)
//{	
//	// Set the operating mode
//	esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
//}
//
///**
//* Function that initializes the SNTP time sync task
//*/
//sntp_clock_status_e sntp_clock_init(void)
//{
//    // Set the local time zone
//    sntp_timeSync_setLocalTimeZone();
//
//    // Set SNTP server
//	esp_sntp_setservername(0, "pool.ntp.org");
////	esp_sntp_config_t sntp_config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
////	esp_netif_sntp_init(&sntp_config);
//	
////	// Let the HTTP server know service is initialized
////	httpServer_monitor_sendMessage(HTTP_TIME_SERVICE_INITIALIZED);
//
//	// Start task
//	sntp_task_init();
//
//	return CLOCK_SNTP_INITIALIZED;
//}
//
//static void sntp_timeSync_obtainTime(void)
//{
//    struct tm time_info = {0};
//    if(sntp_timeSync_isTimeDeprecated(&time_info))
//    {
//        sntp_clock_init();
//        
//	    // Set the local time zone
//	    sntp_timeSync_setLocalTimeZone();
//    }
//}
//
///**
// * Function that returns the last updated LocalTime char
// * @return char* localTime string
// */
//char * sntp_clock_getTime(void){
//    struct tm timeInfo = {0};
//    if(!sntp_timeSync_isTimeDeprecated(&timeInfo))
//    {
//		strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M", &timeInfo);
//		ESP_LOGI(TAG, "Current time info: %s", timeBuffer);
//    }
//    return timeBuffer;
//}
//
//static void sntp_timeSync_setLocalTimeZone(void)
//{
//    setenv("TZ", "BRST+3BRDT+2,M10.3.0,M2.3.0", 1);
//    tzset();    ///> Initialize the timezone conversion rotine
//}
//
//static uint8_t sntp_timeSync_isTimeDeprecated(struct tm * timeInfo)
//{
//	time_t now = 0;
//	time(&now);
//	localtime_r(&now, timeInfo);
//	
//	// Check the time, in case we need to initialize/reinitialize
//    // Is time set? If not, tm_year will be (1970 - 1900).
//	if(timeInfo->tm_year < (2016 - 1900))
//	{
//		ESP_LOGI(TAG, "Time is not set yet");
//		return 1;
//	}
//	return 0;
//}
//
//
//
///**************************
//**	FreeRTOS FUNCTIONS	 **
//**************************/
//
///**
//* The SNTP time synchronization task.
//* @param arg pvParam.
//*/
//static void sntp_timeSync_task(void *pvParam)
//{
//	while (1)
//	{
//		sntp_timeSync_obtainTime();
//		vTaskDelay(30000 / portTICK_PERIOD_MS);
//	}
//
//	vTaskDelete(NULL);
//}
//
///**
//* SNTP task used to update the feature clock.
//* @param pvParameters parameter which can be passed to the task.
//*/
//static void sntp_task_init(void)
//{
//	xTaskCreatePinnedToCore(&sntp_timeSync_task,
//                            "sntp_timeSync_task",
//                            SNTP_TIME_SYNC_TASK_STACK_SIZE,
//                            NULL,
//                            SNTP_TIME_SYNC_TASK_PRIORITY,
//                            NULL,
//                            SNTP_TIME_SYNC_TASK_CORE_ID);
//}