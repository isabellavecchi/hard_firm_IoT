/*
 * menuButton.h
 *
 *  Created on: 19 de nov. de 2024
 *      Author: IVecchiFerreira
 */

#ifndef MAIN_MENUBUTTON_H_
#define MAIN_MENUBUTTON_H_

/**************************
**		  INCLUDES	 	 **
**************************/

// ESP libraries
#include "driver/gpio.h"

// Personal libraries
#include "projectConfig.h"


/**************************
**		DEFINITIONS		 **
**************************/

// Interrupt Handler Function Name
#define INTERRUPT_BUTTON_PUSH_FN(BUTTON) menuButton_ ## BUTTON ## isr_handler

// Interval where a button cannot be pressed after any already has been.
#define DEBOUNCE_DELAY_MS (100) // Debounce delay in milliseconds

// Default Interrupt Flag
#define ESP_INTR_FLAG_DEFAULT	0

#define X_MACRO_APP_BUTTONS_LIST \
	X(MENU_BUTTON, 	MENU_BUTTON_PIN,	GPIO_PULLUP_ENABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_NEGEDGE)\
	X(LEFT_BUTTON, 	LEFT_BUTTON_PIN, 	GPIO_PULLUP_ENABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_NEGEDGE)\
	X(RIGHT_BUTTON, RIGHT_BUTTON_PIN, 	GPIO_PULLUP_ENABLE, GPIO_PULLDOWN_DISABLE, GPIO_INTR_NEGEDGE)



/**************************
**		STRUCTURES		 **
**************************/



/**************************
**		FUNCTIONS		 **
**************************/

/**
 * Turns the menu button on and interrupt configuration.
 * But for now, it should change the radio, or just light up the OLED screen... idk yet.
 */
void menuButton_setup(void);


#endif /* MAIN_MENUBUTTON_H_ */
