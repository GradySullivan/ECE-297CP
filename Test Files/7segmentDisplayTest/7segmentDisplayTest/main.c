/*
 * 7segmentDisplayTest.c
 *
 * Created: 3/2/2021 10:57:00 PM
 * Author : Grady
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DIG1 1
#define DIG2 2
#define DIG3 3 //B1, enables DIG3
#define DIG4 4 //B4, enables DIG4

# define BUTTON 1 //PORTD
int main(void)
{
    unsigned char i, ledDigits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 
		0x07, 0x7F, 0x67};
	unsigned char ledDegrees[] = {0b01110001, 0b00111001};
	unsigned char numRepeats = 200, persistence = 1;
	DDRD = 0xFF;
	DDRB = (1<<DIG1)|(1<<DIG2)|(1<<DIG3)|(1<<DIG4);
	PORTC = (1<<BUTTON); // pullup
    while (1) 
    {
			PORTD = ledDigits[1];
			PORTB = ~(1<<DIG1);
			_delay_ms(persistence);
			PORTD = ledDigits[2];
			PORTB = ~(1<<DIG2);
			_delay_ms(persistence);
			PORTD = ledDigits[3];
			PORTB = ~(1<<DIG3);
			_delay_ms(persistence);
		
		if (!(PINC & (1<<BUTTON))) // test if switch pressed
		{
			PORTD = ledDegrees[1];
			PORTB = ~(1<<DIG4);
			_delay_ms(persistence);
		}
		else
		{
			PORTD = ledDegrees[0];
			PORTB = ~(1<<DIG4);
			_delay_ms(persistence);
		}
    }
}

