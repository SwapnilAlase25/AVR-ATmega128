/*
 * Ext_inter_swtiches.c
 *
 * Created: 14-09-2019 13:18:34
 * Author : Swapnil Alase
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRE= 0x00; //Configure Port E as input port
	PORTE=0xff; //Enable pull up resistor
	
	DDRC=0XFF;	//leds as o/p
	PORTC=0xFF;	
	
	EICRB=0x00;		// low level external Interrupts
	EIMSK=0xC0;		// used int6 and int7
	sei();
	while(1)
	{
		
	}
}
ISR(INT6_vect) //INT6
{
	cli();
	PORTC=0xF7;			// leds
	sei();
}
ISR(INT7_vect) //INT7
{
	cli();
	PORTC=0xFB;
	sei();
}
