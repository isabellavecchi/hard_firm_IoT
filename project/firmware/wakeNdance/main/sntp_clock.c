///*
// * sntp_clock.c
// *
// *  Created on: 23 de nov. de 2024
// *      Author: IVecchiFerreira
// */
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
//#include "esp_system.h"
//#include "esp_event.h"
//#include "esp_log.h"
//#include "esp_attr.h"
//#include "esp_sleep.h"
//#include "nvs_flash.h"
//#include "esp_netif_sntp.h"
//#include "lwip/ip_addr.h"
//#include "esp_sntp.h"
//
//// Personal libraries
//#include "tasks_common.h"
//#include "httpServer.h"
//#include "wifiApp.h"
//#include "sntp_clock.h"
//
//
//
//
///**************************
//**		DECLARATIONS	 **
//**************************/
//
//	/* Variables */
//
//// Tag used for ESP serial console messages
//static const char TAG[] = "nstp_clock";
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
//
//
//
///**************************
//**	FreeRTOS FUNCTIONS	 **
//**************************/
//
///**
// * The SNTP time synchronization task.
// * @param arg pvParam.
// */
//static void sntp_time_sync(void *pvParam)
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
// * SNTP task used to update the feature clock.
// * @param pvParameters parameter which can be passed to the task.
// */
//static void sntp_time_task(void * parameter)
//{
//	xTaskCreatePinnedToCore(&sntp_time_sync, "sntp_time_sync", SNTP_TIME_SYNC_TASK_STACK_SIZE, NULL, SNTP_TIME_SYNC_TASK_PRIORITY, NULL, SNTP_TIME_SYNC_TASK_CORE_ID);
//}
//
///**************************
//**		FUNCTIONS		 **
//**************************/
//
//static void sntp_timeSync_obtainTime(void)
//{
//}
//
///**
// * Function that sets up the SNTP time sync
// */
//void sntp_clock_setup(void)
//{
//}
//
///**
// * Function that initializes the SNTP time sync task
// */
//sntp_clock_status_e sntp_clock_init(void)
//{
//	return CLOCK_SNTP_INITIALIZED;
//}