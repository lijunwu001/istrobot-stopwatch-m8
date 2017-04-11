#include "uart.c"
#include "adc.c"
#include <util/delay.h>
#include <avr/interrupt.h>

int iv;
int start;
long time;
int high;

void send(long value) {
	char c = '0';
	char x = c + (value / 100000UL);
	uart_send(x);
	x = c + (value % 100000UL / 10000UL);
	uart_send(x);
	x = c + (value % 10000UL / 1000UL);
	uart_send(x);
	x = c + (value % 1000UL / 100UL);
	uart_send(x);
	x = c + (value % 100UL / 10UL);
	uart_send(x);
	x = c + (value % 10UL);
	uart_send(x);
}

int bp() {
	return (PINB & (1 << PB0));
}

void init() {
	_delay_ms(1000);
	uart_init(); // UART, RX TX
	init_adc(); // ADC0 - photo resistor
	DDRB &= ~(1 << PB0); // BUTTON
	TIMSK |= (1 << TOIE0); // enable timer 1
	TCCR0 |= (1 << CS01) | (1 << CS00);
	sei();
	// enable global interrupt
	time = 0;
}

void cal() {
	while (!bp())
		;
	while (bp())
		;
	int low = read_adc();

	while (!bp())
		;
	while (bp())
		;
	high = read_adc();
	iv = (high + low) / 2;
}

void sm() {
	char n = '\n'; 
	int x = 0;
	while (1) {
		label1: 
		x = read_adc();
		while (x >= iv){
			x = read_adc();
		}

		start = 1;
		uart_send('S');
		uart_send(n);

		while (x < iv) {
			x = read_adc();
		}
		_delay_ms(1000);
		while (x >= iv) {
			x = read_adc();
			if (bp()) {
				while (bp())
					;
				uart_send('R');
				uart_send(n);
				start = 0;
				time = 0;
				goto label1;
			}
		}

		uart_send('F');
		uart_send(n);
		start = 0;

		while (!bp())
			; // wait for reset
		while (bp())
			;
		uart_send('R');
		uart_send(n);
		start = 0;
		time = 0;
		//_delay_ms(2000);
	}
}

int main(void) {
	init();
	cal();
	sm();
}

ISR( TIMER0_OVF_vect) {
	if (start == 0) {
		return;
	}

	time++;
	uart_send('T');
	send(time);
	uart_send('\n');
}
