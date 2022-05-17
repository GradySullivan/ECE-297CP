/*
 * UART_tester.c
 * Created: 2/11/2021 9:59:08 AM
 * Author : dmcla
 */
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
void uart_init(void);
void uart_send(char);

int main(void){
	uart_init();			// unitialize UART
	unsigned char i;
	char textString[] = "WOW THIS IS A LONG STRING OF TEXT!";
	while (1) {
		for (i=0; i<strlen(textString); i++)
			uart_send(textString[i]);
			uart_send(13);
			uart_send(10);
			_delay_ms(250);
	}
}

void uart_init(void){
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = 103;
}

void uart_send(char letter){
	while (! (UCSR0A & (1<<UDRE0)));
	UDR0 = letter;
}

