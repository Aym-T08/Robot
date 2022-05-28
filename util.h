#ifndef H_UTIL
#define H_UTIL
/**
 * @file util.h
 * @brief 
 * @version 0.1
 * @date 2022-05-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <time.h>
#include "stdio.h"

#define EPSILON_COMPARISON 0.001

#define set_bit(bits,bit) (bits|=_BV(bit))
#define clear_bit(bits,bit) (bits&=~_BV(bit))
#define toggle_led(bit) (PORTA ^= _BV(bit))

#define ENABLE_INTERRUPTS (SREG |= _BV(SREG_I))
#define DISABLE_INTERRUPTS (SREG &= ~_BV(SREG_I))

#define ENABLE_ENCODER (EIMSK |= _BV(INT0))
#define DISABLE_ENCODER (EIMSK &= ~_BV(INT0))

#define ENABLE_SERVOS (EIMSK |= _BV(INT1))
#define DISABLE_SERVOS (EIMSK &= ~_BV(INT1))

/**
 * @brief sets up seed so we can properly use rand()
 * 
 */
void setup_random();

/**
 * @brief generic check whether states are vlid
 * 
 * @param state_a 
 * @param state_b 
 * @return int 
 */
int isValidStates(int state_a, int state_b);



#endif
