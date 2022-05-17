/*
 * tempProject.c
 *
 * Created: 2/28/2021 4:19:35 PM
 * Author : Grady Sullivan
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

void init(void); // initialize everything, contains other init functions
void adc_init(void);
void segment_init(void);

void adc_conversion(void);
void displayTemp(unsigned char ledDigits[], unsigned char tempDigits[], unsigned char ledDegrees);

#define DIG1 1
#define DIG2 2
#define DIG3 3 
#define DIG4 4 //B4, enables DIG4

# define BUTTON 1 //PORTD

int main(void)
{
	//temperature variables
	unsigned int digitalValue;
	float voltInt;
	float tempC;
	float tempF;
	
	//7 segment display variables
	unsigned char ledDigits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
	0x07, 0x7F, 0x67};
	unsigned char ledDegrees[] = {0x71, 0x39}; // Fahrenheit, Celsius
	
	unsigned char F_digit[3]; //array of Fahrenheit digits
	unsigned char C_digit[3]; //array of Celsius digits
	
	init();
	
	while (1)
	{
		adc_conversion();
		digitalValue = ADCL | (ADCH << 8);
		voltInt = digitalValue *1.1/1.024; // voltage in mV
		
		tempC = ((float)(voltInt - 750)/10 +25); // temp in Celsius
		int tempCDisplay = tempC * 10; // get rid of decimal place
		
		tempF = tempC * 9 / 5 + 32; // temp in Fahrenheit
		int tempFDisplay = tempF * 10; // get rid of decimal place
		
		F_digit[0]=(tempFDisplay/100)%100; //ten's place
		F_digit[1]=(tempFDisplay/10)%10; //one's place
		F_digit[2]=tempFDisplay%10; // decimal value
		
		C_digit[0]=(tempCDisplay/100)%100; //tens place
		C_digit[1]=(tempCDisplay/10)%10; //ones place
		C_digit[2]=tempCDisplay%10; //decimal place
		
		if (!(PINC & (1<<BUTTON))) // test if switch pressed
		{
			displayTemp(ledDigits, C_digit, ledDegrees[1]); //temp in C
		}
		else
		{
			displayTemp(ledDigits, F_digit, ledDegrees[0]); //temp in F
		}
	}
}

void init(void)
{
	adc_init();
	segment_init();
	PORTC = (1<<BUTTON); // pullup
}

void adc_init(void) // initialize adc
{
	ADMUX = 0xC0; // Vref = 1.1V
	ADCSRA = 0x87; // enable ADC; set speed to 125KHz
}

void adc_conversion(void)
{
	ADCSRA |= (1 << ADSC); // start ADC conversion
	while ((ADCSRA & (1 << ADIF)) == 0); // wait until ADC finishes
}

void segment_init(void) // set up pins for 7 segment display
{
	DDRD = 0xFF; //7 segment pins high
	DDRB = (1<<DIG1)|(1<<DIG2)|(1<<DIG3)|(1<<DIG4); //enable digits
}

void displayTemp(unsigned char ledDigits[], unsigned char tempDigits[], unsigned char ledDegrees)
{
	unsigned char numRepeats = 100, persistence = 1, i = 0; //loop & timing variables
	
	for(i=0;i<numRepeats;i++)
	{
		// tens digit
		PORTD = ledDigits[tempDigits[0]]; 
		PORTB = ~(1<<DIG1);
		_delay_ms(persistence);

		//ones digit
		PORTD = ledDigits[tempDigits[1]];
		PORTD |= 0x80; //add decimal point
		PORTB = ~(1<<DIG2);
		_delay_ms(persistence);

		//tenths digit
		PORTD = ledDigits[tempDigits[2]];
		PORTB = ~(1<<DIG3);
		_delay_ms(persistence);

		//degrees symbol
		PORTD = ledDegrees;
		PORTB = ~(1<<DIG4);
		_delay_ms(persistence);
	}
}