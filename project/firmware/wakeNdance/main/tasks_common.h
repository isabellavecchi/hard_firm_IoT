/*
 * tasks_common.h
 *
 *  Created on: 16 de nov. de 2024
 *      Author: Luiz Carlos
 */

#ifndef MAIN_TASKS_COMMON_H_
#define MAIN_TASKS_COMMON_H_

// Wifi application task
#define WIFI_APP_TASK_STACK_SIZE		4096
#define WIFI_APP_TASK_PRIORITY			5
#define WIFI_APP_TASK_CORE_ID			0

// HTTP Server task
#define HTTP_SERVER_STACK_SIZE			8192
#define HTTP_SERVER_TASK_PRIORITY		4
#define HTTP_SERVER_TASK_CORE_ID		0

// HTTP Server Monitor task
#define HTTP_SERVER_MONITOR_STACK_SIZE	4096
#define HTTP_SERVER_MONITOR_PRIORITY	3
#define HTTP_SERVER_MONITOR_CORE_ID		0

// Menu Task
#define MENU_TASK_STACK_SIZE			4096
#define MENU_TASK_PRIORITY				6
#define MENU_TASK_CORE_ID				1

// Menu Button Task
#define MENU_BUTTON_TASK_STACK_SIZE		2048
#define MENU_BUTTON_TASK_PRIORITY		6
#define MENU_BUTTON_TASK_CORE_ID		1


// SNTP Time Sync Task
#define SNTP_TIME_SYNC_TASK_STACK_SIZE	4096
#define SNTP_TIME_SYNC_TASK_PRIORITY	4
#define SNTP_TIME_SYNC_TASK_CORE_ID		1

#endif /* MAIN_TASKS_COMMON_H_ */
