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
#include "esp_system.h"
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
#include "esp_timer.h"

// Personal libraries
#include "menuApp.h"
#include "menuButton.h"


/**************************
**		DECLARATIONS	 **
**************************/

	/* Variables */

// Tag used for ESP serial console messages
static const char TAG[] = "menu_button";

// Flag that holds the menu state
static uint8_t fg_isMenuActive = FALSE;

// Var to check if a button can be pressed again, or if it is debouncing period
static uint64_t lastButtonClickedInstant = 0;

	/* FreeRTOS Structures */

// Semaphore handle
SemaphoreHandle_t menu_button_pushed = NULL;

// Timer structures
static esp_timer_handle_t menu_timer;


/* Static Functions */

// App Function
static void menuButton_buttonConfig(void);
static void menuButton_buttonConfigISR(void);
static void menuButton_updateWakeNDanceApp_task(void * pvParam);

// Debouncing Funciton
static uint8_t menuApp_hasDebouncePeriodPassed(void);

// Timer functions
static void menuButton_timer_callback(void *arg);
static void menuButton_timer_setup(void);
static void menuButton_timer_start(void);
static void menuButton_timer_stop(void);
static void menuButton_timer_reset(void);



/**************************
**		APP FUNCTIONS	 **
**************************/

/**
 * ISR handler for the menu (BOOT) button
 * @param arg parameter which can be passed to the ISR handler.
 */
 void IRAM_ATTR INTERRUPT_BUTTON_PUSH_FN(MENU_BUTTON)(void * arg)
{
	if(menuApp_hasDebouncePeriodPassed())
	{
		if(!fg_isMenuActive)
		{
			menuButton_timer_start();
			fg_isMenuActive = TRUE;

			// Send a message to begin the menu app wake and dance state machine
			menuApp_sendMessage(MENU_HOME);
		}
		else
		{
			menuButton_timer_reset();

			// Send an OK message to menu app wake and dance state machine
			menuApp_sendMessage(MENU_OK);
		}
	}
 }

/**
 * ISR handler for the menu (LEFT_BUTTON) button
 * @param arg parameter which can be passed to the ISR handler.
 */
 void IRAM_ATTR INTERRUPT_BUTTON_PUSH_FN(LEFT_BUTTON)(void * arg)
 {
	if(menuApp_hasDebouncePeriodPassed())
	{
		if(fg_isMenuActive)
		{
			// Reset timer
			menuButton_timer_reset();

			// Notify the button task
			menuApp_sendMessage(MENU_N);
		}
		else
		{
			// raise volume up
		}
	}
 }

/**
 * ISR handler for the menu (RIGHT_BUTTON) button
 * @param arg parameter which can be passed to the ISR handler.
 */
 void IRAM_ATTR INTERRUPT_BUTTON_PUSH_FN(RIGHT_BUTTON)(void * arg)
 {
	if(menuApp_hasDebouncePeriodPassed())
	{
		if(fg_isMenuActive)
		{
			// Reset timer
			menuButton_timer_reset();
			
			// Notify the button task
			menuApp_sendMessage(MENU_P);
		}
		else
		{
			// lower the volume
		}
	}
 }

/**
 * Function to configure menu buttons mode, and interrupt type
 */
static void menuButton_buttonConfig(void)
{
	gpio_config_t config_button;
	
	/* Configure the button and set the direction */ 
	#define X(name, pin, pullUpEn, pullDownEn, intEdge)	\
		config_button =	(gpio_config_t)					\
		{												\
			.pin_bit_mask = 1UL << pin,					\
			.mode = GPIO_MODE_INPUT,					\
			.pull_up_en = pullUpEn,						\
			.pull_down_en = pullDownEn,					\
			.intr_type = intEdge,						\
		};												\
		ESP_ERROR_CHECK(gpio_config(&config_button));
	
	X_MACRO_APP_BUTTONS_LIST
	
	#undef X
}

/**
 * Function to configure menu buttons interrupt handlers
 */
static void menuButton_buttonConfigISR(void)
{
	// Install gpio isr service
	gpio_install_isr_service(ESP_INTR_FLAG_LOWMED);
	
	// Attach the interrupt service routine
	#define X(name, pin, pullUpEn, pullDownEn, intEdge)	\
		gpio_isr_handler_add(pin, INTERRUPT_BUTTON_PUSH_FN(name), NULL);\
		gpio_intr_enable(pin);

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
		if (xSemaphoreTake(menu_button_pushed, portMAX_DELAY) == pdTRUE)
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
	// Create a timer, so we can shutdown menu after it is not being used
	menuButton_timer_setup();

	// Create the binary semaphore
	menu_button_pushed = xSemaphoreCreateBinary();
	
	// Configure all the buttons necessary for the menu
	menuButton_buttonConfig();

	// Configure and Enable Buttons press interruption
	menuButton_buttonConfigISR();
	
	// Create the wifi reset button task
	xTaskCreatePinnedToCore(&menuButton_updateWakeNDanceApp_task,
							"menu_button_task",
							MENU_BUTTON_TASK_STACK_SIZE,
							NULL,
							MENU_BUTTON_TASK_PRIORITY,
							NULL,
							MENU_BUTTON_TASK_CORE_ID);
}



/**********************************
**	DEBOUNCE BUTTONS FUNCTION	 **
**********************************/

static uint8_t menuApp_hasDebouncePeriodPassed(void)
{
    uint64_t currentTime = esp_timer_get_time(); 
    uint64_t elapsedTime = (currentTime - lastButtonClickedInstant) / 1000; // Convert to milliseconds

    if (elapsedTime >= DEBOUNCE_DELAY_MS)
	{
        lastButtonClickedInstant = currentTime;
        return TRUE; 
    }
	else
	{
        return FALSE;
    }
}



/**************************
**	  TIMER FUNCTIONS	 **
**************************/

/**
 * Function that handles what should happen after menu_timer timeout.
 * Which in this case is because the menu buttons aren't hit for a while...
 * So the menu is closed.
 */
static void menuButton_timer_callback(void *arg)
{
	menuButton_timer_stop();
    ESP_LOGI(TAG, "Menu App inactive for too long, closing it...\n");
    menuApp_sendMessage(MENU_CLOSE);
	fg_isMenuActive = FALSE;
}

/**
 * Function that creates the timer and sets its callback function.
 */
static void menuButton_timer_setup(void)
{
    // Configurar o callback do timer
    esp_timer_create_args_t args = {
        .callback = menuButton_timer_callback,
        .name = "menu_timer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&args, &menu_timer));
}

/**
 * Function that starts the timer, until stop or timeout at TIMEOUT_MENU_INACTIVE_MILLIS.
 */
static void menuButton_timer_start(void)
{
    ESP_ERROR_CHECK(esp_timer_start_periodic(menu_timer, TIMEOUT_MENU_INACTIVE_MICROSECONDS));
}

/**
 * Function that stops the timer menu_timer count down.
 */
static void menuButton_timer_stop(void)
{
	ESP_ERROR_CHECK(esp_timer_stop(menu_timer));
}

/**
 * Function that resets the timer menu_timer count down.
 */
static void menuButton_timer_reset(void)
{
	menuButton_timer_stop();
	menuButton_timer_start();
}