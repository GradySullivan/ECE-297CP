/*
 * pullupButtonTest.c
 *
 * Created: 3/2/2021 9:16:31 PM
 * Author : Grady
 */ 

#define F_CPU 16000000
#include <avr/io.h>

#define BLUE_LED 0 // PORT B
#define BUTTON 1 //PORT D

int main(void)
{
	DDRB = (1<<BLUE_LED); //PB0 is output; other pins input
	PORTC = (1<<BUTTON); // activate pullup on PC1
    while (1) 
    {
		if (!(PINC & (1<<BUTTON))) // test if switch is pressed
			PORTB |= (1<<BLUE_LED); // set PB0 high (LED on)
		else
			PORTB &= ~(1<<BLUE_LED); // set PB0 low (LED off)
    }
}

