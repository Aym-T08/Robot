/**
 * @file usart.h
 * @brief  Bluetooth communication related functions and macros
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef H_USART
#define H_USART
#include "util.h"

/**
 * @brief in this "mode" when communicating with pc we send actions to perform from pc
 * 
 */
#define MANUAL_MODE 10 

/**
 * @brief in this "mode" when communicating with pc we will just visualise on pc, no further iteraction
 * 
 */
#define NO_MANUAL_MODE 19 

/**
 * @brief used to make sure master and slave are in sync
 * 
 */
#define ACKNOWLEDGE 13 


/**
 * @brief used to make sure master and slave are in sync
 * 
 */
#define TRY_TO_SYNC 100

/**
 * @brief send data
 * 
 * @param data 
 */
void sendData(int data);

/**
 * @brief receive data and return the data by reference (argument)
 * 
 * @param data 
 */
void receiveData(int* data);
/**
 * @brief initialise usart
 * 
 */
void init_usart();

/**
 * @brief used in communication with pc to receive an action to perform
 * 
 * @param action 
 * @return int 
 */
int process_master_action(int* action);


/**
 * @brief used to receive choice from pc
 * 
 * @param choice 
 */
void process_master_choice(int* choice);

#endif
