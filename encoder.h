/**
 * @file encoder.h
 * @brief Encoder header containing relevant structs and functions
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef H_ENCODER
#define H_ENCODER

#include "util.h" // includes avr/io and everything we need



#define ENABLE_ENCODER (EIMSK |= _BV(INT0))
#define DISABLE_ENCODER (EIMSK &= ~_BV(INT0))
#define ENCODER_STATUS 0

/**
 * @brief struct used in encoder interrupt (INT0) to count pulses, in previous we save the last 2 pins (PD1|PD0)
 *        in curernt we save the last 2, this way we can find out which sequence it belongs to and find out if it's a positive or negative count
 */
typedef struct encoder_info {
    volatile uint16_t previous: 2;
    volatile uint16_t current: 2;
    volatile uint16_t value: 8;
} encoder_info;

/**
 * @brief reset previous encoder value and start counting
 * 
 */
void start_counting_encoder();

/**
 * @brief get latest encoder value
 * 
 * @return char 
 */
char get_encoder_value();

/**
 * @brief not in use as we are using 20ms pulse width, but with a 4ms pulse width the time between interpolation was too quick
 *        so we were required to let the encoder count for a bit longer, however that is not needed right now. still useful for future incase
 *        we swap back
 * @return char 
 */
char calculate_raw_reward();  

/**
 * @brief set up the encoder, sets and clers the correct pins and initialise encoder struct (we use PD0 and PD1 to read our encoder values)
 * 
 */
void setup_encoder_sensor();

#endif
