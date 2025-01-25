/*
 * menuApp.h
 *
 *  Created on: 19 de nov. de 2024
 *      Author: IVecchiFerreira
 */

#ifndef MAIN_MENUAPP_H_
#define MAIN_MENUAPP_H_


/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries

// ESP libraries
#include "freertos/portmacro.h"


/**************************
**		DEFINITIONS		 **
**************************/

// Time to close menu app after nothing happens, in milliseconds
#define TIMEOUT_MENU_INACTIVE_MICROSECONDS 10000000

#define MENU_STATE_FUNC_NAME(state) menuApp_sm_ ## state ## _fn

#define X_MACRO_MENU_STATE_LIST \
	X(MENU_INIT)\
	X(MENU_HOME)\
	X(MENU_OK)	\
	X(MENU_P)\
	X(MENU_N)	\
	X(MENU_CLOSE)\
	X(MENU_DT_UPDATE)



/**************************
**		STRUCTURES		 **
**************************/

/**
 * Message IDs for the Menu application task
 */
typedef enum sm_menu_app_state
{
	#define X(STATE) STATE,
		X_MACRO_MENU_STATE_LIST
	#undef X
} sm_menu_app_state_e;

/**
 * Messages for each Menu application task State
 */
static const char * sm_menu_app_state_names[] = 
{
#define X(STATE) #STATE,
	X_MACRO_MENU_STATE_LIST
#undef X
};

/**
 * Structure for the message queue
 */
typedef struct menu_app_queue_message_s
{
    sm_menu_app_state_e  menu_state;
} menu_app_queue_message_t;


// Defining a type for every function of the Menu state machine
typedef void (*sm_menu_app_function)(menu_app_queue_message_t * st);

// Creating a struct to hold those functions later on an array
typedef struct sm_menu_table_fn_s
{
	sm_menu_app_function func;
} sm_menu_table_fn_t;




/**************************
**		FUNCTIONS		 **
**************************/

/**
 * Starts the WiFi RTOS task
 */
void menuApp_start(void);

/**
 * Sends a message to the queue
 * @param msgId message ID from the menu_app_message_e enum
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE
 * @TODO colocar um enum status pra indicar falha ou sucesso
 */
BaseType_t menuApp_sendMessage(sm_menu_app_state_e msgId);


#endif /* MAIN_MENUAPP_H_ */
