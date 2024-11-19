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
	
	// Web Router
	router_setup();
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