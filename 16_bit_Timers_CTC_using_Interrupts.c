/*
 * Timer_with_interrupts.c
 *
 * Created: 04-09-2019 13:38:44
 * Author : Swapnil Alase
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000

unsigned int count=0;

ISR(TIMER1_OVF_vect)				//isr for ctc timer1
{
	count++;
	if(count==3)
	{
		PORTC = ~PORTC;
		count=0;
		TCNT1=0X00;
	}
}

int main(void)
{
	DDRC = 0XFF;
	PORTC=0XFF;
	TCCR1A=0X00;
	TCCR1B |= (1<<CS11);		// 16/8 prescalar
	TCNT1=0X00;					//delay of 100 ms
	TIMSK |= (1<<TOIE1);       // enable overflow interrupt
	sei();
	
	while(1)
	{

	}
}
