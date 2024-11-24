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
#include <stdint.h>

// ESP libraries

// Personal libraries



/**************************
**		DEFINITIONS	 	 **
**************************/

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 48
#endif

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
 * A function that informs if the localtime is informed or not.
 */
uint8_t router_isLocalTimeSet(void);

/**
 * Starts the SNTP server synchronization task.
 */
void sntp_timeSync_task_start(void);

#endif /* MAIN_SNTP_TIMESYNC_H_ */
