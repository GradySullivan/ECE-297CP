/*
 * toggleProject.c
 *
 * Created: 2/10/2021 11:50:01 AM
 * Author : Grady Sullivan
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include "util/delay.h"

int blink(int pin_num);
void blinkBoth(void);

int main(void)
{
    DDRB = (1<<0)|(1<<1); // pins PB0 and PB1 are output pins
	
    while (1) 
    {
		if (PIND&(1<<0)) // if button is pressed...
			blink(0); // blink the green LED
		else
			PORTB &= ~(1<<0); // turn green LED off 
			
		if (PIND&(1<<1)) // if button is pressed...
			blink(1); // blink the red LED
		else
			PORTB &= ~(1<<1); // turn red LED off

		if (PIND&(1<<2)) // if button is pressed...
		{
			blinkBoth();
		}
		else
		{
			PORTB &= ~(1<<1); // turn both LEDs off	
			PORTB &= ~(1<<0);	
		}
    }
}

int blink (int pin_num)
{
	PORTB |= (1<<pin_num); // turn pin on --> turn on LED
	_delay_ms(100);
	PORTB &= ~(1<<pin_num); // turn pin off --> turn off LED
	_delay_ms(100); // total of 200 ms delay, so this will loop 5 times a second, or 5 Hz
}

void blinkBoth (void)
{
	PORTB |= (1<<0); // turn PB0 on --> turn on green LED
	PORTB |= (1<<1); // turn PB1 on --> turn on red LED
	_delay_ms(100);
	PORTB &= ~(1<<0); // turn PB0 off --> turn off green LED
	PORTB &= ~(1<<1); // turn PB1 off --> turn off red LED
	_delay_ms(100);
}

