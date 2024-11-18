/*
 * ledRGB.h
 *
 *  Created on: 15 de nov. de 2024
 *      Author: Luiz Carlos
 */

#ifndef MAIN_LIB_LEDRGB_H_
#define MAIN_LIB_LEDRGB_H_


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries
#include <stdio.h>



/**************************
**		DEFINITIONS		 **
**************************/

#define TRUE  1
#define FALSE 0

#define LED_PWM_SPEED LEDC_LOW_SPEED_MODE
#define LED_PWM_TIMER_INDEX LEDC_TIMER_1

// RGB LED GPIOs
#define RGB_LED_RED_GPIO	4
#define RGB_LED_GREEN_GPIO	5
#define RGB_LED_BLUE_GPIO	6
//#define RGB_LED				38                                                                              

// RGB LED color mix channels
#define RGB_LED_CHANNEL_NUM	3

// X MACRO for LED
#define X_MACRO_LED \
	X(0, RGB_LED_RED_GPIO,		LED_PWM_SPEED, LED_PWM_TIMER_INDEX) \
	X(1, RGB_LED_GREEN_GPIO,	LED_PWM_SPEED, LED_PWM_TIMER_INDEX) \
	X(2, RGB_LED_BLUE_GPIO,		LED_PWM_SPEED, LED_PWM_TIMER_INDEX)



/**************************
**		STRUCTURES		 **
**************************/

// RGB LED configuration
typedef struct ledRGB_info_s
{
	uint8_t channel;
	uint8_t gpio;
	uint8_t mode;
	uint8_t timerIndex;
} ledRGB_info_t;



/**************************
**		FUNCTIONS		 **
**************************/

/**
 * Initializes the RGB LED settings per channel, including
 * the GPIO for each color, mode and timer configuration.
 */
void ledRGB_ledPWM_init(void);

/**
 * Color to indicate Wifi application has started.
*/
void ledRGB_wifiApp_started(void);

/**
 * Color to indicate HTTP server has started.
*/
void ledRGB_httpServer_started(void);


/**
 * Color to indicate that the ESP32 is connected to an access point.
*/
void ledRGB_wifi_connected(void);

/**
 * Color to indicate that the ESP32 has disconnected from access point.
*/
void ledRGB_wifi_disconnected(void);

#endif /* MAIN_LIB_LEDRGB_H_ */
