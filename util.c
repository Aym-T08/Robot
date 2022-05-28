#include "util.h"
//setup
void setup_random() {
    srand(time(NULL));
}

int isValidStates(int state_a, int state_b) { 
    if (state_a >= 0 && state_a < 4 && state_b >= 0 && state_b < 4) { 
        return 1;
    }
    else {
        return 0;
    }
}

