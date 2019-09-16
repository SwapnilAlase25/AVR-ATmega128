/*
 * ip_uart_op-lcd.c
 *
 * Created: 14-09-2019 10:17:11
 * Author : Swapnil Alase
 */ 

// input from uart and display on 16x2 lcd

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define F_CPU 16000000 // set clock speed
#define BAUD 9600 // baud rate
#define MYUBRR F_CPU/16/BAUD-1 // calculate baud rate




void lcd_init();
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void clear_bit(unsigned int);
void set_bit(unsigned int);
void enable();
void lcd_string(char *);

int flag=0;

void usart_init(unsigned int ubrr)
{
	//usart init
	/*set baud rate*/
	UBRR1H = (unsigned char) (ubrr>>8);
	UBRR1L = (unsigned char) ubrr;
	/*enable receiver & transmitter*/
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);
	/* set frame rate format: 8 data  bit and 2 stops bits*/
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
	while (text[i]!='\0')
	{
		
		usart_transmit (text[i++]);
	}
	
	usart_transmit(0x0A);  //\N
	usart_transmit(0x0D);  // \R
	lcd_cmd(0x02);
	lcd_cmd(0x01);
	
	
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
	
	while (i<10)
	{
		text[i++] = usart_receive();
		if(text[i-1] == '\r')
		{text[i-1]='\0';
		break;}
	}
	flag=1;
}

int main(void)
{
	char string[20];
	
	lcd_init();
	usart_init(MYUBRR);
 
		while(1)
		{
			receive_text(string);
			transmit_text(string);
			_delay_ms(100);
			if(flag==1)
			{
				flag=0;
				lcd_cmd(0x01);
			lcd_string(string);
			_delay_ms(100);
			}
		}

}

void lcd_init()
{
DDRA=0XFF;			//output


lcd_cmd(0x28);		//4 bit data and 2 line mode
clear_bit(0);
enable();

lcd_cmd(0x0c);		//display on
lcd_cmd(0x01);		//clear screen
lcd_cmd(0x80);		//1st line and 1st char	
//lcd_cmd(0x02);		//return to home position
}

void lcd_cmd(unsigned char ch)
{
	PORTA= (ch & 0xf0 );
	clear_bit(0);
	enable();
	ch=ch<<4;
	PORTA =(ch & 0xf0);
	clear_bit(0);
	enable();
	
}

void lcd_data(unsigned char ch)
{
PORTA=(ch & 0xF0);
set_bit(0);
enable();
ch=ch<<4;
PORTA=(ch & 0xF0);
set_bit(0);
enable();

}

void clear_bit(unsigned int bit)
{
PORTA=PORTA & (~(1<<bit));
}

void set_bit(unsigned int bit)
{
PORTA=PORTA | (1<<bit);	
}

void enable()
{
	set_bit(2);
	_delay_ms(20);
	clear_bit(2);
	_delay_ms(20);
	
}

void lcd_string(char *str)
{
while(*str!='\0')
{
	lcd_data(*str++);
}
	
}
