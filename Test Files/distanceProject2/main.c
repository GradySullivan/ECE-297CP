/*
 * distanceProject.c
 *
 * Created: 3/21/2021 1:40:50 PM
 * Author : Grady
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DIG1 1 //C1
#define DIG2 2 //C2
#define DIG3 3 //C3
#define DIG4 4 //C4

#define TRIGGER 1 //B1
#define LED 2 //B0

double preLoad;

void init(void);
int getDistance(void);
void getDigits(unsigned int distance, unsigned int* distanceDigits);
void displayDistance(unsigned int* distanceDigits);
void displayDigit(unsigned char distanceDigits, unsigned char place);
void blinker(double);

int main(void)
{
	unsigned int distanceDigits[4];
	unsigned int distance;
	init();
	double frequency = 1;
	
	while (1)
	{
		distance = getDistance();
		
		if (distance > 200)
			frequency = 0.5;
		else if (distance < 10)
			frequency = 25;
		else
			frequency = 21 - 1./10 * distance;
		
		blinker(frequency);
		getDigits(distance, distanceDigits);
		displayDistance(distanceDigits);
	}
}

void init()
{
	DDRD = 0xFF; // set 7-segment LEDs to output
	DDRC = (1<<DIG1)|(1<<DIG2)|(1<<DIG3)|(1<<DIG4); // set digit as outputs
	DDRB = (1<<LED)|(1<<TRIGGER); // set LED and TRIGGER as outputs
	
	TCCR1A = 0; //initialize timer 1 on normal mode
	TCCR1B = (1<<CS12)|(1<<CS10); // set 1024 prescalar to timer 1's clock
	TIMSK1 = (1<<TOIE1); // enable timer 1 overflow interrupt
	sei();
}

int getDistance()
{
	unsigned int timeToRisingEdge, timeToFallingEdge, pulseWidth, distance;
	
	while(1)
	{
		PORTB |= 1<< TRIGGER;
		_delay_us(10.);
		PORTB &= ~(1 << TRIGGER);
		
		TCCR1B |= (1<<ICES1); // set timer to capture on rising edge of ICP1
		while ((TIFR1 & (1 << ICF1)) == 0); // wait for rising edge to be captured
		timeToRisingEdge = ICR1; // set variable to captured timestamp in register
		TIFR1 = (1 << ICF1); // clear capture flag
		
		TCCR1B &= ~(1<<ICES1); // set timer to capture on falling edge of ICP1 pin
		while ((TIFR1 & (1 << ICF1)) == 0)
		timeToFallingEdge = ICR1;
		TIFR1 = (1 << ICF1);
		
		if (timeToFallingEdge > timeToRisingEdge) break;
	}
	
	pulseWidth = timeToFallingEdge - timeToRisingEdge;
	distance = pulseWidth * 1.098;
	
	return distance;
}

void getDigits(unsigned int distance, unsigned int* distanceDigits)
{
	distanceDigits[0] = (distance/1000)%10; // thousands place
	distanceDigits[1] = (distance/100)%10;  // hundreds place
	distanceDigits[2] = (distance/10)%10;   // tens place
	distanceDigits[3] = (distance%10);      // ones place
}

void displayDistance(unsigned int* distanceDigits)
{	
	for(int i=0;i<50;i++)
	{
		displayDigit(distanceDigits[0], DIG1);
		displayDigit(distanceDigits[1], DIG2);
		displayDigit(distanceDigits[2], DIG3);
		displayDigit(distanceDigits[3], DIG4);
	}
}

void displayDigit(unsigned char distanceDigits, unsigned char place)
{
	unsigned char persistence = 3;
	unsigned char ledDigits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
	0x07, 0x7F, 0x67};
	
	PORTD = ledDigits[distanceDigits];
	PORTC = ~(1<<place);
	_delay_ms(persistence);
	PORTC |= 0xFF;
}

void blinker(double frequency)
{
	preLoad = 65536-7812. / frequency;
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = preLoad;
	PORTB ^= (1<<LED);
}