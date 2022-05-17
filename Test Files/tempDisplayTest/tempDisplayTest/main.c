/*
 * tempDisplayTest.c
 *
 * Created: 3/4/2021 9:19:35 AM
 * Author : grady
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

void uart_init(void);
void uart_send(char letter);
void send_string(char *stringAddress);
void adc_init(void);
void blink(void);
void adc_conversion(void);
void segment_init(void);

#define BLUE_LED 0 //PORT B
#define DIG1 1
#define DIG2 2
#define DIG3 3 //B1, enables DIG3
#define DIG4 4 //B4, enables DIG4

# define BUTTON 1 //PORTD
int main(void)
{
	//temp variables
	unsigned int digitalValue;
	float voltInt;
	float tempC;
	float tempF;
	char buffer[6];
	int F_digit[3];
	int C_digit[3];
	
	//7 segment display variables
	unsigned char i, ledDigits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
	0x07, 0x7F, 0x67};
	unsigned char ledDegrees[] = {0b01110001, 0b00111001};
	unsigned char numRepeats = 100, persistence = 3;
	
	adc_init();
	segment_init();
	
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
		
		C_digit[0]=(tempCDisplay/100)%100; //ten's place
		C_digit[1]=(tempCDisplay/10)%10; //one's place
		C_digit[2]=tempCDisplay%10; //decimal place
		
		if (!(PINC & (1<<BUTTON))) // test if switch pressed
		{
			for(i=0;i<numRepeats;i++)
			{
				PORTD = ledDigits[C_digit[0]];
				PORTB = ~(1<<DIG1);
				_delay_ms(persistence);
				PORTD = ledDigits[C_digit[1]];
				PORTD |= 0b10000000; //decimal point
				PORTB = ~(1<<DIG2);
				_delay_ms(persistence);
				PORTD = ledDigits[C_digit[2]];
				PORTB = ~(1<<DIG3);
				_delay_ms(persistence);
				PORTD = ledDegrees[1];
				PORTB = ~(1<<DIG4);
				_delay_ms(persistence);
			}
		}
		else
		{
			for(i=0;i<numRepeats;i++)
			{
				PORTD = ledDigits[F_digit[0]];
				PORTB = ~(1<<DIG1);
				_delay_ms(persistence);
				PORTD = ledDigits[F_digit[1]];
				PORTD |= 0b10000000; //decimal point
				PORTB = ~(1<<DIG2);
				_delay_ms(persistence);
				PORTD = ledDigits[F_digit[2]];
				PORTB = ~(1<<DIG3);
				_delay_ms(persistence);
				PORTD = ledDegrees[0];
				PORTB = ~(1<<DIG4);
				_delay_ms(persistence);
			}
		}
		
	}
}

void uart_init(void) //initialize uart
{
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0L = 103;
}

void uart_send(char letter) //send single ASCII character
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = letter;
}

void send_string(char *stringAddress) //send string of ASCII characters
{
	unsigned char i;
	for (i = 0; i < strlen(stringAddress); i++)
	uart_send(stringAddress[i]);
}

void adc_init(void) // initialize adc
{
	ADMUX = 0b11000000; // Vref = 1.1V
	ADCSRA = 0x87; // enable ADC; set speed to 125KHz
}

void blink(void) // led blink test
{
	PORTB |= (1<<BLUE_LED); // set PB0
	_delay_ms(250);
	PORTB &= ~(1<<BLUE_LED);
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
	PORTC = (1<<BUTTON); // pullup
}

