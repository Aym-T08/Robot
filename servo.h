/**
 * @file servo.h
 * @brief Servo related functions and macros
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef H_SERVO
#define H_SERVO

#include "util.h"


#define SHORT_DELAY 3 // delay inbetween interpolation steps 2*20ms
#define LONG_DELAY 30 // delay after all interpolation steps have been completed 20*20ms

#define DELTA_A 800 // steps inbetween servo values in servo a
#define DELTA_B 1000 // steps inbetween servo values in servo b
#define INTERPOL_STEPS 25

/**
 * @brief struct containing info to move servos around and interpolate, incase we wanna chnge easily
 * 
 */
typedef struct servo_info { 
    unsigned short delta_a;
    unsigned short delta_b;
    unsigned char steps;
    unsigned char delta_a_step;
    unsigned char delta_b_step; 
} servo_info;

/**
 * @brief moves the servo according to given states
 * 
 * @param state_a 
 * @param state_b 
 */
void moveServos(int state_a, int state_b); 


/**
 * @brief  timer to delay for max_ticks * 20ms (value in top), more efficient than defining an entire new timer for the same purpose..
 * 
 * @param max_ticks 
 */
void servo_timer_delay(int max_ticks); 

/**
 * @brief activate servo timer counter
 *  
 */
void start_counting();  
/**
 * @brief deactive servo timer counter
 * 
 */
void stop_counting(); 

/**
 * @brief get current ticks of timer
 * 
 * @return unsigned char 
 */
unsigned char get_ticks(); 


/**
 * @brief Sets up servos with given steps to interpolate
 * 
 * @param steps 
 */
void setup_servos(char steps); 


/**
 * @brief moves the servos and interpolates according to current servo_info
 * 
 * @param pos_x 
 * @param pos_y 
 */
void move_servos_and_interpolate(int pos_x, int pos_y); 

#endif
