/*
 * ledRGB.c
 *
 *  Created on: 15 de nov. de 2024
 *      Author: Luiz Carlos
 */


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries
#include <stdio.h>

// ESP libraries
#include "driver/ledc.h"

// Personal libraries
#include "ledRGB.h"


/**************************
**		DECLARATIONS	 **
**************************/

// RGB LED Configuration Array
ledRGB_info_t led_channels[RGB_LED_CHANNEL_NUM];



/**************************
** GPIO & channel config **
**************************/

/**
 * Initializes the ledRGB_info_t struct, with
 * the GPIO for each color, mode and timer configuration.
 */
 static void ledRGB_info_init(ledRGB_info_t * ledRGB_info_v, uint8_t channel, uint8_t gpioPin, uint8_t mode, uint8_t timerIndex)
 {
	ledRGB_info_v->channel		= channel;
	ledRGB_info_v->gpio			= gpioPin;
	ledRGB_info_v->mode			= mode;
	ledRGB_info_v->timerIndex	= timerIndex;
 }

/**
 * sets RGB LED channel, including
 * the GPIO for each color, mode and timer configuration.
 */
 static void ledRGB_config(uint8_t channel, uint8_t gpioPin, uint8_t mode, uint8_t timerIndex)
 {
	ledc_channel_config_t ledc_channel = 
		{
			.channel	= channel,
			.duty		= 0,
			.hpoint		= 0,
			.gpio_num	= gpioPin,
			.intr_type	= LEDC_INTR_DISABLE,
			.speed_mode	= mode,
			.timer_sel	= timerIndex, 
		};
		ledc_channel_config(&ledc_channel);
 }



/**************************
**		APP FUNCTIONS	 **
**************************/

/**
 * Initializes the RGB LED settings per channel, including
 * the GPIO for each color, mode and timer configuration.
 */
 void ledRGB_ledPWM_init(void)
 {
	 #define X(channel, pin, speedMode, timerIndex) \
	 	ledRGB_info_init(&(led_channels[channel]), LEDC_CHANNEL_##channel, pin, speedMode, timerIndex);
	 	X_MACRO_LED
	 #undef X

	 // Configure timer zero
	 ledc_timer_config_t ledc_timer =
	 {
		 .duty_resolution	= LEDC_TIMER_8_BIT,
		 .freq_hz			= 100, //hertz
		 .speed_mode		= LED_PWM_SPEED,
		 .timer_num			= LED_PWM_TIMER_INDEX
	 };
	 ledc_timer_config(&ledc_timer);
	 
	 // Configure channels
	 #define X(channel, pin, speedMode, timerIndex) \
	 	ledRGB_config(LEDC_CHANNEL_##channel, pin, speedMode, timerIndex);
	 	X_MACRO_LED
	 #undef X
 }
 
 /**
 * Sets the RGB color.
 */
 static void ledRGB_setColor(uint8_t red, uint8_t green, uint8_t blue)
 {
	 ledc_set_duty(led_channels[0].mode, led_channels[0].channel, red);
	 ledc_update_duty(led_channels[0].mode, led_channels[0].channel);

	 ledc_set_duty(led_channels[1].mode, led_channels[1].channel, green);
	 ledc_update_duty(led_channels[1].mode, led_channels[1].channel);
	 
	 ledc_set_duty(led_channels[2].mode, led_channels[2].channel, blue);
	 ledc_update_duty(led_channels[2].mode, led_channels[2].channel);
 }
 
 /**
 * Color to indicate Wifi application has started.
*/
void ledRGB_wifiApp_started(void)
{
	ledRGB_setColor(255, 102, 255);
}

/**
 * Color to indicate HTTP server has started.
*/
void ledRGB_wifi_disconnected(void)
{
	ledRGB_setColor(204, 255, 51);
}

/**
 * Color to indicate that the ESP32 is connected to an access point.
*/
void ledRGB_wifi_connected(void)
{
	ledRGB_setColor(0, 255, 153);
}

/**
 * Color to indicate that the ESP32 has disconnected from access point.
*/
void ledRGB_wifi_disconnect(void)
{
	ledRGB_setColor(199, 0, 0);
}