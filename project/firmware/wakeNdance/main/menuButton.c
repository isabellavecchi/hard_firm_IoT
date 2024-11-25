/*
 * menuButton.c
 *
 *  Created on: 19 de nov. de 2024
 *      Author: IVecchiFerreira
 */


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries

// ESP libraries
#include "esp_intr_alloc.h"
#include "esp_rom_gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "tasks_common.h"

// Personal libraries
#include "menuApp.h"
#include "menuButton.h"


/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "menu_button";

// Semaphore handle
SemaphoreHandle_t menu_button_semaphore = NULL;


/* Static Functions */

// App Function
static void menuButton_buttonConfig(void);
static void menuButton_updateWakeNDanceApp_task(void * pvParam);



/**************************
**		APP FUNCTIONS	 **
**************************/

/**
 * ISR handler for the menu (BOOT) button
 * @param arg parameter which can be passed to the ISR handler.
 */
 void IRAM_ATTR menuButton_isr_handler(void * arg)
 {
	 // Notify the button task
	 xSemaphoreGiveFromISR(menu_button_semaphore, NULL);
 }

/**
 * Function to configure menu buttons mode, and interrupt type
 */
static void menuButton_buttonConfig(void)
{
	#define X(pin, pinMode, intEdge) \
	/* Configure the button and set the direction */ \
	esp_rom_gpio_pad_select_gpio(MENU_BUTTON); \
	gpio_set_direction(MENU_BUTTON, GPIO_MODE_INPUT); \
	\
	/* Enable interrupt on the negative edge */ \
	gpio_set_intr_type(MENU_BUTTON, GPIO_INTR_NEGEDGE);
	
	X_MACRO_APP_BUTTONS_LIST
	
	#undef X
}
 
/**
 * Menu button task reacts to a BOOT button event by sending a update message
 * to the OLED display app, and the radio App.
 * @param pvParam parameter which can be passed to the task
 */
static void menuButton_updateWakeNDanceApp_task(void * pvParam)
{
	for(;;)
	{
		if (xSemaphoreTake(menu_button_semaphore, portMAX_DELAY) == pdTRUE)
		{
			ESP_LOGI(TAG, "MENU BUTTON INTERRUPT OCCURED");
			
			// Send a message to begin the menu app wake and dance state machine
			menuApp_sendMessage(MENU_HOME);
			
			// @TODO: na verdade só posso voltar aqui quando nenhum botão for pressionado por alguns segundos
			vTaskDelay(2000/portTICK_PERIOD_MS);
		}
	}
}

/**
 * Turns the menu buttons on and sets interrupt configuration.
 */
void menuButton_setup(void)
{	
	// Create the binary semaphore
	menu_button_semaphore = xSemaphoreCreateBinary();
	
	// Configure all the buttons necessary for the menu
	menuButton_buttonConfig();
	
	// Create the wifi reset button task
	xTaskCreatePinnedToCore(&menuButton_updateWakeNDanceApp_task,
							"menu_button_task",
							MENU_BUTTON_TASK_STACK_SIZE,
							NULL,
							MENU_BUTTON_TASK_PRIORITY,
							NULL,
							MENU_BUTTON_TASK_CORE_ID);
	
	// Install gpio isr service
	gpio_install_isr_service(ESP_INTR_FLAG_LOWMED);
	
	// Attach the interrupt service routine
	gpio_isr_handler_add(MENU_BUTTON, menuButton_isr_handler, NULL);
}