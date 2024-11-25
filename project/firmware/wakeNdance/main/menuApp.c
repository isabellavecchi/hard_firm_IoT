/*
 * menuApp.c
 *
 *  Created on: 19 de nov. de 2024
 *      Author: IVecchiFerreira
 */


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries
#include <stdio.h>

// ESP libraries
#include "esp_event.h"
#include "esp_event_base.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/portmacro.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"

// Personal libraries
#include "tasks_common.h"
#include "menuApp.h"


/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "menu_app";

// Variable to check if the state Id is valid
static uint8_t g_qtd_states;


	/* FreeRTOS Structure */

// Queue handle used to manipulate the main queue of events
static QueueHandle_t menu_app_queue_handle_t;

// Timer structures
static esp_timer_handle_t menu_timer;


	/* Static Functions */

// Timer functions
static void menuApp_timer_callback(void *arg);
static void menuApp_timer_setup(void);
static void menuApp_timer_start(void);
static void menuApp_timer_stop(void);

// State Machine functions
static void MENU_STATE_FUNC_NAME(MENU_INIT)(menu_app_queue_message_t * st);
static void MENU_STATE_FUNC_NAME(MENU_HOME)(menu_app_queue_message_t * st);
static void MENU_STATE_FUNC_NAME(MENU_CLOSE)(menu_app_queue_message_t * st);
static void menuApp_stateMachine_handler(menu_app_queue_message_t * msg);

// App functions
static void menuApp_setup(void);
static void menuApp_task(void * pvParameters);



/**************************
**	  TIMER FUNCTIONS	 **
**************************/

/**
 * Function that handles what should happen after menu_timer timeout.
 * Which in this case is because the menu buttons aren't hit for a while...
 * So the menu is closed.
 */
static void menuApp_timer_callback(void *arg)
{
	menuApp_timer_stop();
    ESP_LOGI(TAG, "Menu App inactive for too long, closing it...\n");
    menuApp_sendMessage(MENU_CLOSE);
}

/**
 * Function that creates the timer and sets its callback function.
 */
static void menuApp_timer_setup(void)
{
    // Configurar o callback do timer
    esp_timer_create_args_t args = {
        .callback = menuApp_timer_callback,
        .name = "menu_timer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&args, &menu_timer));
}

/**
 * Function that starts the timer, until stop or timeout at TIMEOUT_MENU_INACTIVE_MILLIS.
 */
static void menuApp_timer_start(void)
{
    ESP_ERROR_CHECK(esp_timer_start_periodic(menu_timer, TIMEOUT_MENU_INACTIVE_MICROSECONDS));
}

/**
 * Function that stops the timer menu_timer count down.
 */
static void menuApp_timer_stop(void)
{
	ESP_ERROR_CHECK(esp_timer_stop(menu_timer));
}



/**************************
**		STATE MACHINE	 **
**************************/

/**
 * State Machine Function Definition according to sm_menu_app_function
 * function that defines the behavior on 
 * [MENU_INIT] state
 */
static void MENU_STATE_FUNC_NAME(MENU_INIT)(menu_app_queue_message_t * st)
{	
	ESP_LOGI(TAG, "MENU_INIT");
	
	// Reset timer
	menuApp_timer_start();
}

/**
 * State Machine Function Definition according to sm_menu_app_function
 * function that defines the behavior on 
 * [MENU_HOME] state
 */
static void MENU_STATE_FUNC_NAME(MENU_HOME)(menu_app_queue_message_t * st)
{	
	ESP_LOGI(TAG, "MENU_HOME");
	
	// Reset timer
	menuApp_timer_start();
}

/**
 * State Machine Function Definition according to sm_menu_app_function
 * function that defines the behavior on 
 * [MENU_END] state
 */
static void MENU_STATE_FUNC_NAME(MENU_CLOSE)(menu_app_queue_message_t * st)
{	
	ESP_LOGI(TAG, "MENU_CLOSE");
}


// State Machine Table Functions - to be used in the loop
sm_menu_table_fn_t sm_menu_state_table[] =
{
#define X(ENUM) { MENU_STATE_FUNC_NAME(ENUM) }, 
	X_MACRO_MENU_STATE_LIST
#undef X
};

static void menuApp_stateMachine_handler(menu_app_queue_message_t * msg)
{
	g_qtd_states = 0;
	#define X(ENUM) g_qtd_states++; 
		X_MACRO_MENU_STATE_LIST
	#undef X
	
	for (;;)
	{
		if (xQueueReceive(menu_app_queue_handle_t, msg, portMAX_DELAY))
		{
			if(msg->menu_state < g_qtd_states)
			{
				sm_menu_state_table[msg->menu_state].func(msg);
			}
		}
	}
}


/**************************
**	   APP FUNCTIONS	 **
**************************/

 /**
  * function that sets up the WiFi environment
  * configurating WiFi & Soft Access and initializing TCP/IP
  */
static void menuApp_setup(void)
{	
	// Create a timer, so we can shutdown menu after it is not being used
	menuApp_timer_setup();
	
	// Setup the OLED display
//	oledDisplay_setup();
}



/**************************
**	FreeRTOS FUNCTIONS	 **
**************************/
 
/**
 * Main task for the WiFi application
 * where we initialize WiFi and and receive all queue messages
 * which determine the flow of the application
 * @param pvParameters parameter which can be passed to the task
 */
static void menuApp_task(void * pvParameters)
{
	menu_app_queue_message_t msg;
	
	// Setup WiFi environment
	menuApp_setup();
	
	// Send first event message
	menuApp_sendMessage(MENU_INIT);
	
	menuApp_stateMachine_handler(&msg);
}

/**
 * Sends a message to the queue
 * @param msgId message ID from the menu_app_message_e enum
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE
 * @TODO colocar um enum status pra indicar falha ou sucesso
 */
BaseType_t menuApp_sendMessage(sm_menu_app_state_e msgId)
{
	menu_app_queue_message_t msg;
	msg.menu_state = msgId;
	return xQueueGenericSend(menu_app_queue_handle_t, &msg, portMAX_DELAY, queueSEND_TO_BACK );
}

/**
 * Starts the WiFi RTOS task
 */
void menuApp_start(void)
{
	ESP_LOGI(TAG, "STARTING MENU APPLICATION");
	
	// Turn OLED display ON at menu mode
//	oledDisplay_menuApp_started();
	
	// Create message queue
	menu_app_queue_handle_t = xQueueCreate(3, sizeof(menu_app_queue_message_t));
	
	// Start the Menu application task
	xTaskCreatePinnedToCore(	&menuApp_task,
								"menuApp_task",
								MENU_TASK_STACK_SIZE,
								NULL,
								MENU_TASK_PRIORITY,
								NULL,
								MENU_TASK_CORE_ID);
							
}