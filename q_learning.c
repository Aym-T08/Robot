#include "q_learning.h"

volatile float table[STATES][STATES][ACTION];

// using unsigned char as we require no more than that as datatype
// for rewards use int incase we every wanna count diff in future
volatile unsigned char state_a = 0;
volatile unsigned char state_b = 0;
volatile unsigned char state_a_now = 0;
volatile unsigned char state_b_now = 0;
volatile unsigned char action = 0;
volatile unsigned char random_action = 0;
volatile int final_reward = 0;
volatile int iteration = 0;
volatile int raw_reward = 0;

void update_qtable() {
    // what is the largest possible q value the robot can receive in the future (as in, considering all possible VALID actions)
    float highest_q_val_future = look_forward(state_a_now, state_b_now);

    // how much of our previously calculated q values should we consider in the future 
    float part_1 = (1-ALPHA) * table[state_a][state_b][action];

    // potential q values in the future (potential reward, and look ahead step);
    float part_2 = ALPHA * (final_reward + GAMMA * highest_q_val_future);

    // update q value in OLD state pair in the action that resulted moving from old state pair to new
    table[state_a][state_b][action] = part_1 + part_2;

    // save old states
    state_a = state_a_now; 
    state_b = state_b_now;

    iteration++;
}


void send_qtable_comms() {
    sendData(state_a_now);
    sendData(state_b_now);
    sendData(state_a);
    sendData(state_b);
    sendData(raw_reward);
    sendData(action);
    sendData(random_action);
}

void qtable_set_action(unsigned char action_new) {
    action = action_new;
}

void init_qtable() {
    state_a = 1; state_a_now = 1; state_b = 1; state_b_now = 1;
    action = 1; final_reward = 0; iteration = 0; raw_reward = 0;
    int i;
    int j;
    int k;
    for (i = STATES - 1; i >= 0; --i) 
    {
        for (j = STATES - 1; j >= 0; --j) 
        {
            for (k = ACTIONS - 1; k >= 0; --k) 
            {
                table[i][j][k] = 0.0F;
            }
        }
    } 
}


void perform_e_greedy() {
    int chance = rand() % 100;
    if  (chance > (1-EPSILON)*100   || iteration < RAND_ITERATIONS) {
        action = getRandomAction(state_a_now, state_b_now);
        random_action = 1;
    }  
    else {
        
        int highest_i = ACTIONS-1;
        for (int i = 2; i >= 0; --i) {
            if(table[state_a_now][state_b_now][highest_i] < table[state_a_now][state_b_now][i]) {
                highest_i = i;
            }
        }   
        random_action = 0;
        action = highest_i;
    }
}

void process_qtable_action() {
    unsigned char new_action = 0;
    unsigned char x = state_a_now; unsigned char y = state_b_now;
    action_to_states(&x, &y, action); // perform last action, if valid, continue
    if (!isValidStates(x, y)) {
        while (!isValidStates(x, y)) { // if not valid, perform a random action from old state pair, which was same as initial state_a_now/state_b_now
            x = state_a; y = state_b;
            new_action = rand()%ACTIONS; 
            action_to_states(&x, &y, new_action);  
        }
        random_action = 1;
        action = new_action;
    }
    state_a_now = x; // update latest states
    state_b_now = y;
}

void set_reward_qtable(char reward) {
    raw_reward = reward;
}

unsigned char get_qtable_action() {
    return action;
}

unsigned char get_qtable_state_b() {
    return state_a_now;
}

unsigned char get_qtable_state_a() {
    return state_b_now;
}

void process_raw_reward() {
    final_reward = raw_reward;
    final_reward -= 5;  // punish standing still
    if(final_reward > 0) {
        final_reward *= final_reward;
    }
    if (final_reward < - 7) {
        final_reward = -70;
    }
}



unsigned char getRandomAction(unsigned char state_a_t, unsigned char state_b_t) {
    int flag = 0;
    int action_t = 0; 
    unsigned char x = 0; unsigned char y = 0;
    do {
        action_t = rand()%4;
        x = state_a_t; y = state_b_t;
        action_to_states(&x, &y, action_t);
        if (isValidStates(x, y)) {
            flag = 1;
        }
    } while (flag == 0);
    return action_t;
}

float look_forward(unsigned char state_a_t, unsigned char state_b_t) {
    float q_waarde = -1000.0;
    float q_temp = 0.0;
    unsigned char x = state_a_t;
    unsigned char y = state_b_t;
    for (int action_t = 0; action_t < ACTIONS; action_t++) {
        x = state_a_t; y = state_b_t;
        action_to_states(&x, &y, action_t);
        if (isValidStates(x, y)) {
            q_temp = largest_q_value(x, y);
            if (q_temp > q_waarde) {
                q_waarde = q_temp;
            }
        }
    }
    return q_waarde;
}

float largest_q_value(unsigned char state_a_t, unsigned char state_b_t) {
    float q_waarde = table[state_a_t][state_b_t][ACTIONS-1];
    for (int i = ACTIONS-2; i >= 0; --i) {
        if(q_waarde < table[state_a_t][state_b_t][i]) {
            q_waarde = table[state_a_t][state_b_t][i];
        }
    }
    return q_waarde;
}

void action_to_states(unsigned char *state_a_t, unsigned char *state_b_t, unsigned char action) {
    switch(action) {
        case UP:
            *state_a_t -= 1;
            break;

        case DOWN:
            *state_a_t += 1;            
            break;

        case LEFT:
            *state_b_t -= 1;          
            break;

        case RIGHT:
            *state_b_t += 1;
            break;
    }
}
