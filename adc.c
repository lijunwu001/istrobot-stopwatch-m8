/*
 * adc.c
 *
 *  Created on: Apr 6, 2017
 *      Author: Lijun
 */

#include <avr/io.h>

void init_adc(){
	ADCSRA |= (1 << ADEN);
	ADMUX &= 0xF8;
}

unsigned char read_adc(){
	ADCSRA |= (1<<ADSC);
	while ((ADCSRA & (1<<ADSC)));
	return ADC;
}
