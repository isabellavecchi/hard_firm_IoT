///*
// * sntp_clock.h
// *
// *  Created on: 23 de nov. de 2024
// *      Author: IVecchiFerreira
// */
//
//#ifndef MAIN_SNTP_CLOCK_H_
//#define MAIN_SNTP_CLOCK_H_
//
///**************************
//**		  INCLUDES	 	 **
//**************************/
//
//// C libraries
//#include <stdint.h>
//
//
//
///**************************
//**		DEFINITIONS	 	 **
//**************************/
//
//
//
///**************************
//**		STRUCTURES		 **
//**************************/
//
/////**
//// * Enum that indicates the current state of the time synchronization
//// */
////typedef enum sntp_clock_status
////{
////	CLOCK_SNTP_INITIALIZED = 0, 
////	CLOCK_SNTP_GET_SUCCESS,
////	CLOCK_SNTP_CONNECTION_NOT_FOUND,
////	CLOCK_SNTP_ERROR,
////} sntp_clock_status_e;
////
/////**
//// * Structure for the message queue
//// */
////typedef struct sntp_clock_s
////{
////	uint8_t fg_initialized;
////    sntp_clock_status_e  menu_state;
////} sntp_clock_t;
//
//
//
///**************************
//**		DEFINITIONS	 	 **
//**************************/
//
///**
// * Function that sets up the SNTP time sync
// */
//void sntp_clock_setup(void);
//
///**
// * Function that initializes the SNTP time sync task
// */
//sntp_clock_status_e sntp_clock_init(void);
//
///**
// * Function that returns the last updated LocalTime char
// * @return char* localTime string
// */
//char * sntp_clock_getTime(void);
//
//#endif /* MAIN_SNTP_CLOCK_H_ */