//*
 * uart_string_rxtx.c
 *
 * Created: 13-09-2019 14:28:29
 * Author : Swapnil Alase
 TeraTerm is used on the other side
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define F_CPU 16000000 // set clock speed
#define BAUD 9600 // baud rate
#define MYUBRR F_CPU/16/BAUD-1 // calculate baud rate

void usart_init(unsigned int ubrr)
{
	//usart init
	/*set baud rate*/
	UBRR1H = (unsigned char) (ubrr>>8);
	UBRR1L = (unsigned char) ubrr;		 
	/*enable receiver & transmitter*/
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);
	/* set frame rate format: 8 data  bit*/
	UCSR1C =  (1<<USBS0) | (3<<UCSZ00);
}

void usart_transmit(unsigned char data)
{
	// Wait if a byte is being transmitted
	while(!(UCSR1A & (1<<UDRE1)));
	// Transmit data
	UDR1 = data;
}

void transmit_text(char text[])
{
	int i = 0;
	usart_transmit(0x0A);  //\N
	usart_transmit(0x0D);  // \R
	while (i<32)			// 16*2 lcd can print max 32 char
	{
		if( text[i] == '\0' )
		break;
		usart_transmit (text[i++]);
	}
	usart_transmit(0x0A);  //\N
	usart_transmit(0x0D);  // \R
	
	
}

unsigned char usart_receive(void)
{
	// Wait until a byte has been received
	while((UCSR1A&(1<<RXC1)) == 0);
	// Return received data
	return UDR1;
}

void receive_text (char text[])
{
	int i = 0;
	while (i<32)
	{
		text[i++] = usart_receive();
		if(text[i-1] == '\r')			// if \r recibed from teraterm then 
		{
		text[i-1]='\0'; 				// replace \r with \0
		break;
		}
	}
}

int main(void)
{
	char string[20];

	usart_init(MYUBRR);				// uart_init

	while (1)
	{	
		receive_text(string);
		transmit_text(string);
	}
}
