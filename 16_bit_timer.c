/*
 * Timers.c
 *
 * Created: 02-09-2019 04:53:35
 * Author : Swapnil Alase
 */ 


#include <avr/io.h>

#define F_CPU 16000000

void delay_ms(unsigned int);


int main(void)
{
	DDRC=0XFF;
    
    while (1) 
    {
		PORTC=0X0F;
		delay_ms(100);
		PORTC=0xF0;
		delay_ms(100);
    }
}
 void delay_ms(unsigned int n)
 {
	 TCCR1B=0X02;			//SET PRESCALAR 16/8
	 TCCR1A=0X00;			//NORMAL MODE
	 TCNT1=0XF830;			//  1 MILLI SECS  65535-1999=63,537 is f830
	 
	for(int i=0;i<n;i++)
	 {
	 while((TIFR&(1<<2))!=0X04);			// till overflow flag doesn't get set
		TCNT1=0xF830;
		TIFR=0X04;				// clear register 
	 }
 }


