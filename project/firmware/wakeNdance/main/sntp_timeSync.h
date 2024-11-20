/*
 * nstp_tymeSync.h
 *
 *  Created on: 20 de nov. de 2024
 *      Author: IVecchiFerreira
 */

#ifndef MAIN_SNTP_TIMESYNC_H_
#define MAIN_SNTP_TIMESYNC_H_



/**************************
**		  INCLUDES	 	 **
**************************/

// C libraries

// ESP libraries

// Personal libraries


/**************************
**		DEFINITIONS		 **
**************************/




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
 * Setup the local timezone and operating mode
 */
void sntp_timeSync_setup(void);

/**
 * Setup the local timezone and operating mode
 */
void sntp_timeSync_init(void);

/**
 * Returns local time if set.
 * @return local time buffer.
 */
char* sntp_timeSync_getTime(void);

/**
 * Starts the SNTP server synchronization task.
 */
void sntp_timeSync_task_start(void);

#endif /* MAIN_SNTP_TIMESYNC_H_ */
