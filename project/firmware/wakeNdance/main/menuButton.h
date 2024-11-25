/*
 * menuButton.h
 *
 *  Created on: 19 de nov. de 2024
 *      Author: IVecchiFerreira
 */

#ifndef MAIN_MENUBUTTON_H_
#define MAIN_MENUBUTTON_H_


/**************************
**		DEFINITIONS		 **
**************************/

// Default Interrupt Flag
#define ESP_INTR_FLAG_DEFAULT	0

// Menu Button is the BOOT button on the chip
#define MENU_BUTTON				0
#define LEFT_BUTTON				
#define RIGHT_BUTTON

#define X_MACRO_APP_BUTTONS_LIST \
	X(MENU_BUTTON, GPIO_MODE_INPUT, GPIO_INTR_NEGEDGE)



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
