#ifndef __LIJUNIUS_UART_
#define __LIJUNIUS_UART_

// define some macros
#define F_CPU 8000000UL
#define BAUD 38400                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

#include <avr/io.h>

// function to initialize UART
void uart_init (void)
{
    UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRRL = BAUDRATE;                           // set baud rate
    UCSRB = (1<<TXEN)|(1<<RXEN);                // enable receiver and transmitter
    UCSRC |= (1<<URSEL) | (1 << UCSZ0) | (1<<UCSZ1);   // async 8bit data format 1-stop bits
}

void uart_send(unsigned char data){
        while (!( UCSRA & (1<<UDRE)));                // wait while register is free
        UDR = data;
}

unsigned char uart_receive(){
        while(!(UCSRA & (1<<RXC)));                   // wait while data is being received
        return UDR;
}
#endif
