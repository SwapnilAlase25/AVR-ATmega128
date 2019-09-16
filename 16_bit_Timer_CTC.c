/*
 * timer_ctc.c
 *
 * Created: 06-09-2019 02:13:34
 * Author : Swapnil Alase
 */ 

//CTC= CLEAR TIMER ON COMPARE
#include <avr/io.h>


int main(void)
{
	DDRC=0xff;
	TCCR1A=0x00;
	TCNT1=0x00;					// timer start from 0
	OCR1A = 19999;					//for delay of 100 ms
	TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10); //CTC MODE AND 16/64 PRESCLAR
	
    
	while (1) 
    {
		
		if(TIFR & (1<<OCF1A))			//when OCF1A is high
		{
			PORTC=~PORTC;			//toggle leds
		}
		
		TIFR |= (1<<OCF1A); 			// clear the OCF1A (in the hardware architecture, for clearing set againg the same bit)
		
		
    }
}

