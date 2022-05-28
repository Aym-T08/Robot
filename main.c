#include "encoder.h"
#include "q_learning.h"
#include "servo.h"
#include "util.h"
#include "usart.h"

#define COMMS 0 // if we wanna communicate with PC and visualise, set this to 1, else to 0

int main(void) {
    setup_servos(INTERPOL_STEPS);
    setup_encoder_sensor();
    init_usart();
    init_qtable();
    ENABLE_SERVOS;
    DISABLE_ENCODER;
    set_bit(SREG,SREG_I); 
    int choice = 0;
    unsigned char action = 0;
    int reward = 0;
    int wait_for_master = 0;
    unsigned char state_x = 1; unsigned char state_y = 1;
    for(;;) {
        if (COMMS) { // if communicating with pc, have to make sure pc and microcontroller are synchronized
            process_master_choice(&choice);
            if (choice == MANUAL_MODE) { // transmit actions from pc
                process_master_action(&action); 
                qtable_set_action(action);
            }
            else if (choice == NO_MANUAL_MODE) { // don't transmit actions, but visualise the data step by step
                receiveData(&wait_for_master); 
                perform_e_greedy();
                sendData(ACKNOWLEDGE);          // acknowledge slave received choice, master asks for action or proceeds
            }
        }
        // decide whether we should perform a random action next iteration, or calculate the "optimal" one ourselves
        if (!COMMS) {perform_e_greedy();} // if comms is set to 0, pretend we are in "no manual mode" and just perform algorithm

        // performs the latest action (and handles potential out of bounds errors), updates the current states accordingly
        process_qtable_action();
        
        // get current states so we can move the servos
        state_x = get_qtable_state_a();
        state_y = get_qtable_state_b();

        // turns on sensors and calculates by how much the bot has moved forward/backwards
        start_counting_encoder();

        // move the servos based on current state pair
        move_servos_and_interpolate(state_x, state_y);

        // recover distance from encoder and stop the encoder, then set the reward as raw_reward in q table
        reward = get_encoder_value();
        set_reward_qtable(reward);

        // sends all appropriate q table info to be visualised on pc
        send_qtable_comms();

        // turns raw_reward into final_reward (-3 for standing still, ..)
        process_raw_reward(); 

        // update q table, update old q values with look ahead step and its old values.
        update_qtable();
    }   
}