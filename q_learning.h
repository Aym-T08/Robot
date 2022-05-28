/**
 * @file q_learning.h
 * @brief Functions and macros related to Q learning algorithm
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef H_Q_LEARNING
#define H_Q_LEARNING

#include "util.h"
#include "usart.h"

/**
 * @brief used to transmit data order in which we transmit the data), has to match pc 
 * 
 */
#define STATE_A_NOW 0
#define STATE_B_NOW 1
#define STATE_A 2
#define STATE_B 3
#define BELONING 4
#define ACTION 5
#define RAND_ACTION 6

/**
 * @brief parameters of q algorithm
 * 
 */
#define EPSILON 0.1
#define GAMMA 0.99
#define ALPHA 0.5

/**
 * @brief amount of random iterations (used in e_greedy action decision)
 * 
 */
#define RAND_ITERATIONS 100

#define STATES 4
#define ACTIONS 4

/**
 * @brief type of actions
 * 
 */
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

/**
 * @brief initialise the q table
 * 
 */
void init_qtable(); 

/**
 * @brief send data to pc for visualisation
 * 
 */
void send_qtable_comms(); 

/**
 * @brief decide whether we should perform a random action next iteration, or calculate the "optimal" one ourselves
 * 
 */
void perform_e_greedy();  

/**
 * @brief sets action in q table (used when action is sent from pc)
 * 
 * @param action_new 
 */
void qtable_set_action(unsigned char action_new); 

/**
 * @brief performs the latest action (and handles potential out of bounds errors), updates the current states accordingly
 * 
 */
void process_qtable_action(); 

/**
 * @brief update q table, update old q values with look ahead step and its old values. 
 * 
 */
    void update_qtable();  

/**
 * @brief return a random action based on given states
 * 
 * @param state_a 
 * @param state_b 
 * @return unsigned char 
 */
unsigned char getRandomAction(unsigned char state_a, unsigned char state_b); 

/**
 * @brief get look ahead step, used in updating q value
 * 
 * @param state_a 
 * @param state_b 
 * @return float 
 */
float look_forward(unsigned char state_a, unsigned char state_b); 

/**
 * @brief returns largest q value in given state pair 
 * 
 * @param state_a 
 * @param state_b 
 * @return float 
 */
float largest_q_value(unsigned char state_a, unsigned char state_b); 

/**
 * @brief updates states according to action (by reference)
 * 
 * @param state_a 
 * @param state_b 
 * @param action 
 */
void action_to_states(unsigned char *state_a, unsigned char *state_b, unsigned char action); 

/**
 * @brief sets reward in q table, used when we have gotten reward from encoder
 * 
 * @param reward 
 */
void set_reward_qtable(char reward); 

/**
 * @brief self explanatory getter functions, used for convenience so we can move servos around easily from main
 * 
 * @return unsigned char 
 */
unsigned char get_qtable_state_b(); 
unsigned char get_qtable_state_a(); 
unsigned char get_qtable_action();

/**
 * @brief turns raw reward into final reward and saves it
 * 
 */
void process_raw_reward(); 

#endif
