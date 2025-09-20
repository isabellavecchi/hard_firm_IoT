/**
 * Application Entry Point.
 */

/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries

// ESP libraries
#include "esp_err.h"
#include "nvs.h"
#include "nvs_flash.h"

// Personal libraries
#include "ledRGB.h"
#include "router.h"
#include "dateTimeNTP.h"
#include "menuApp.h"
#include "menuButton.h"
#include "displayOled.h"
#include "i2s.h"


/**************************
**		DECLARATIONS	 **
**************************/


/**************************
**		 FUNCTIONS		 **
**************************/

void app_main(void)
{
	// Initialize NVS (Non Volatile Storage)
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	
	// Initialize the LEDS
	ledRGB_ledPWM_init();

	// Initialize the display OLED	
	displayOled_setup();

	// Web Router
	router_setup();

	// NTP clock setup
	dateTimeNTP_setup();
	
	// Menu from button Setup
	menuApp_start();
	
	// Setting up the buttons handler for menu
	menuButton_setup();

	// Sound Task Start
	i2s_start();

}


//task to be created
void vTaskCode(void * pvParameters)
{
	for(;;)
	{
		//task code goes here
	}
}

//xTaskCreate(vTaskCode, "NAME", STACK_SIZE, &ucParametersToPass, tskIDLE_PRIORITY, &xHandle);


////handling errors
//esp_err_t err;
//do {
//	err = sdio_slave_send_queue(addr, len, arg, timeout);
//	//keep retrying while the sending queue is full
//} while (err == ESP_ERR_TIMEOUT);
//if (err != ESP_OK) {
//	//handle other errors
//}
