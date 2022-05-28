#include "usart.h"

void sendData(int data) {
    while( !( UCSR1A & (1<<UDRE1)) ) // make sure tx send flag is set so we can process next data
        ;
    UDR1 = data;
}

void receiveData(int* data) {
    while (!(UCSR1A & (1<<RXC1))); // make sure receive complete flag is set
    *data = (int) UDR1; // all values we work with are ints so we mght as well cast it here
}

void init_usart() {
    uint16_t ubrr_value = 103;
	UBRR1 = ubrr_value;
	/* TX en RX aanzetten */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* 8 data, 1 stop*/
	UCSR1C = (1<<UCSZ10)|(1<<UCSZ11);
}

int process_master_action(int* action) {
    receiveData(action);
        toggle_led(PA2);  // 2 OFF
        toggle_led(PA3);  // 3 ON
    sendData(ACKNOWLEDGE);
        toggle_led(PA3);  // 3 OFF
}

void process_master_choice(int* choice) {
    sendData(TRY_TO_SYNC);         // master sync when this byte received
        toggle_led(PA0);  // 0 ON
    receiveData(choice);          // wait until master transmits choice
        toggle_led(PA0);  // 0 OFF
        toggle_led(PA1);  // 1 ON
    sendData(ACKNOWLEDGE);          // acknowledge slave received choice, master asks for action or proceeds
        toggle_led(PA1);  // 1 OFF
        toggle_led(PA2);  // 2 ON
}
