#include "servo.h"

volatile servo_info servos;

volatile unsigned char counter_ticks = 0;
volatile char servo_counter_active = 0;

/* these PWM values were experimentally calculated (instead of (0.7ms/20ms)*39999 where 39999 is value in ICR1 (top) (and 2.3ms)
    reason: robot couldn't move forward due to robot not being able to fully extend serv arm (something in the way
    the middle values were calculated as so: (MAX-MIN)*(degrees/180) + MIN
*/

volatile int servo_pos_A[4] = {2200, 3000, 3800, 4600};
volatile int servo_pos_B[4] = {1500, 2500, 3500, 4500};

void moveServos(int state_a, int state_b) {
    DISABLE_ENCODER;
    if (isValidStates(state_a, state_b)) {
        OCR1A = servo_pos_A[state_a];
        OCR1B = servo_pos_B[state_b];
    }
    ENABLE_ENCODER;
}

void servo_timer_delay(int max_ticks) {
    start_counting();
    while (get_ticks() < max_ticks);
}

void start_counting() {
    servo_counter_active = 1;
    counter_ticks = 0;
}

void stop_counting() {
    servo_counter_active = 0;
}


unsigned char get_ticks() {
    return counter_ticks;
}

void setup_servos(char steps) { 
    
    servos.delta_a = DELTA_A;
    servos.delta_b = DELTA_B;
    servos.steps = steps;
    servos.delta_a_step = servos.delta_a / steps;
    servos.delta_b_step = servos.delta_b / steps;

    servo_counter_active = 0;
    counter_ticks = 0;
    // 20 ms ((16M / 8 * 50Hz) - 1, where 50Hz is the frequency corresponding 20ms, 16 our cpu speed and 8 the prescaler setting
    // we use fast PWM
    ICR1 = 39999;
    TCNT1 = 0;
     
    DDRB |= (_BV(PB5) | _BV(PB6));     /* set ddr oc1a oc1b to 1 */
    DDRC |= _BV(PC0);
    DDRC |= _BV(PC1);
    
    set_bit(TCCR1A,COM1A1);   /* clear OC1A on compare and set when top is reached */
    clear_bit(TCCR1A,COM1A0);
    set_bit(TCCR1A,COM1B1);   /* clear OC1B on compare and set when top is reached */
    clear_bit(TCCR1A,COM1B0);

    set_bit(TCCR1B,WGM13);    /* matches FAST PWM */
    set_bit(TCCR1B,WGM12);    
    set_bit(TCCR1A,WGM11);    
    clear_bit(TCCR1A,WGM10);    

    clear_bit(TCCR1B,CS12);     /* prescaler op 8 */
    set_bit(TCCR1B,CS11);
    clear_bit(TCCR1B,CS10);

    set_bit(TIMSK1, TOIE1);   /* activate TOV1 interrupt */
    set_bit(TIMSK1, OCIE1A); 
    set_bit(TIMSK1, OCIE1B);
}

ISR(TIMER1_COMPA_vect) {
    PORTC &= ~_BV(PC0); /* servo 1 */
}

ISR(TIMER1_COMPB_vect) {
    PORTC &= ~_BV(PC1); /* servo 2 */
}

ISR(TIMER1_OVF_vect) {
    PORTC |= _BV(PC0);
    PORTC |= _BV(PC1); 
    if(servo_counter_active) { // servo counter, used in counter if larger delay is needed, useful so we don't have to use another timer
        counter_ticks++;
    } 
}


void move_servos_and_interpolate(int pos_x, int pos_y)
{
    int OCR1A_nu = OCR1A;
    int OCR1B_nu = OCR1B;
    for (int i = 0; i < servos.steps + 1; i++) {
        DISABLE_ENCODER;
        if (OCR1A_nu != servo_pos_A[pos_x]) {  // if a isn't equal, there must be a change in servo A, else it's B
            if (OCR1A_nu < servo_pos_A[pos_x]) {
                OCR1A += servos.delta_a_step;
            }
            else {
                OCR1A -= servos.delta_a_step;
            }
        }
        else {
            if (OCR1B_nu < servo_pos_B[pos_y]) {
                OCR1B += servos.delta_b_step;
            }
            else {
                OCR1B -= servos.delta_b_step;
            }
        }
        ENABLE_ENCODER;
        servo_timer_delay(SHORT_DELAY); // delay so we got extra time to count encoder 
    }
    moveServos(pos_x, pos_y); // when we have moved servos with interpolation, move to right value just to make sure we aren't off out of bounds
    servo_timer_delay(LONG_DELAY); // delay so we got extra time to count encoder 
}
