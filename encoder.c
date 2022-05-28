/**
 * @file encoder.c
 * @brief Encoder file containing functions and interrupt
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "encoder.h"
#include "servo.h" // we included this so we can utilise the timer servo is attached to as a delay counter instead of using a new timer

volatile encoder_info encoder; // global struct containing the previous 2 pins, the current 2 pins (PIN D 1, PIN D 0), and the pulses counted

/**
 * @brief   encoder_output containing all possible sequences of pin layouts
            compare previous 2 pin (PD1, PD0) and current one and then update pulse accordingly (only 8 possible sequences)
            there are 2 possible directions the pins can be sampled: (each number represents a 2 bit number consisting of pin
            PD1 and PD0)
            1 direction:
            0 1 3 2, 
            other direction:
            2 3 1 0, 
            now if we look at any number, for example 1, we see that the next number can only be 3, or 0
                    
            0 1 .3. 2
            2 3 1 .0.

            so we know that if our first count is 1, and then we get a 3, the number 
            we get when shifting it into 1 number 
                (1<<2)|3 which is 7
            or  (1<<2)|0 which is 4
            now at position 7 in encouder_output we have a 1, and at position 4 we have a -1
            which is exactly what we are looking for! the max number we can have is 2^4-1 (4 bits)
            so we can have n out of bounds index, and if we get a wrong value we associate it with 0 as it might be a 
            double count 
        
 * 
 */
ISR(INT0_vect){
    const char encoder_output[16] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};
    encoder.current = (PIND&3);
    if(encoder_output[((encoder.previous<<2) | encoder.current)] == -1) {
        if (encoder.value > -127) { // char, prevent underflow
            encoder.value--;
        }  
    }
    else if(encoder_output[((encoder.previous<<2) | encoder.current)] == 1) {
        if (encoder.value < 127) { // char, prevent overflow
            encoder.value++;
        }
    }      
    encoder.previous = encoder.current;
}


char calculate_raw_reward() {
    servo_timer_delay(50); // delay for 50 * 32 ms, this is for how long we will be calculating reward for 
    EIMSK &= ~_BV(INT0);
    char val = encoder.value;
    encoder.value = 0;
    return val;
}   

void start_counting_encoder() {
    encoder.value = 0;
    ENABLE_ENCODER;
}



char get_encoder_value() {
    DISABLE_ENCODER;
    return encoder.value;
}

void setup_encoder_sensor() {
    // turn pins as output
    DDRD &= ~_BV(DDD0); 
    DDRD &= ~_BV(DDD1);

    // these are the pins we read
    PORTD = _BV(PD0)|_BV(PD1); 
    
    // falling and rising edge
    EICRA &= ~_BV(ISC01); 
    EICRA |= _BV(ISC00);

    encoder.current = 0; 
    encoder.previous = 0;
    encoder.value = 0;
}

