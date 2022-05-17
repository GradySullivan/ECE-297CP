/*
 * tempTest.c
 *
 * Created: 3/2/2021 3:41:06 PM
 * Author : Grady
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

void uart_init(void);
void uart_send(char letter);
void send_string(char *stringAddress);

#define BLUE_LED 0 //PORT B

int main(void)
{
	unsigned int digitalValue;
	int voltInt;
	int tempC;
	int tempF;
	char buffer[6];
	DDRC = 0x00;
	ADMUX = 0b11000000;
	ADCSRA = 0x87;
	uart_init();
	DDRB = (1<<BLUE_LED); //PB0 is output; other pins input
	while (1) 
    {
		PORTB |= (1<<BLUE_LED); // set PB0
		_delay_ms(250);
		PORTB &= ~(1<<BLUE_LED);
		
		ADCSRA |= (1 << ADSC);	
		while ((ADCSRA & (1 << ADIF)) == 0);
		digitalValue = ADCL | (ADCH << 8);
		itoa(digitalValue, buffer, 10);
		send_string(buffer);
		uart_send(' ');
		
		voltInt = digitalValue *1.1/1.024;
		itoa(voltInt, buffer, 10);
		send_string(buffer);
		
		uart_send(' ');
		
		float tempC = (voltInt - 750)/10 +25; //temp in C
		itoa(tempC, buffer, 10);
		send_string(buffer);
		
		int tempF = tempC * 9 / 5 + 32; //temp in F
		itoa(tempC, buffer, 10);
		send_string(buffer);
		
		_delay_ms(1000);	
    }
	return 0;
}

void uart_init(void)
{
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0L = 103;
}

void uart_send(char letter)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = letter;
}

void send_string(char *stringAddress)
{
	unsigned char i;
	for (i = 0; i < strlen(stringAddress); i++)
		uart_send(stringAddress[i]);
}

