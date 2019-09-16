/*
 * ADC_LCD_UART_ISR.c
 *
 * Created: 14-09-2019 01:58:48 PM
 * Author : Swapnil Alase
 */ 

// Input from adc which was getting input from ldr, output on serial terminal and 16*2 lcd

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

float adc_val;

void en_pulse()
{	
	PORTA|=(1<<PA2);
	_delay_ms(5);
	PORTA&=~(1<<PA2);
	_delay_ms(5);
}
void lcd_cmd(unsigned char cmd)
{
	PORTA=(cmd & 0xF0);
	PORTA&=~(1<<PA0);
	en_pulse();
	
	cmd=cmd<<4;
	PORTA=(cmd & 0xF0);
	PORTA&=~(1<<PA0);
	en_pulse();
}
void lcd_data(unsigned char cmd)
{	
	PORTA=(cmd & 0xF0);
	PORTA |=(1<<PA0); 
	en_pulse();
	
	cmd=cmd<<4;
	PORTA=(cmd & 0xF0);
	PORTA|=(1<<PA0);
	en_pulse();
}

void lcd_string(char *str)
{
	while(*str!='\0')
		lcd_data(*str++);	
}
void lcd_init()
{
		DDRA=0xFF;		//op
		_delay_ms(20);
		
		lcd_cmd(0x02);		//4 bit mode
		lcd_cmd(0x28);		//5x7 dot, 2 line mode
		lcd_cmd(0x0E);		//disp ON cursor Blinking
		lcd_cmd(0x01);		//clear disp
		lcd_cmd(0x80);		//Force cursor to beginning ( 1st line)
}

void uart_init()
{
	UBRR1L=103;
	UCSR1B|=(1<<RXEN1)|(1<<TXEN1)|(1<<TXCIE1);
}


ISR(USART1_TX_vect)
{
	
}
void adc_init()
{
	ADMUX|=(1<<REFS1)|(1<<REFS0);						//right shifted result, internal 2.56v vref
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADFR)|(1<<ADEN)|(1<<ADIE);	// adc0 select, free running, enable adc
	ADCSRA|=(1<<ADSC);
}

void convert_bits(int c)					//function to convert int voltage to presentable uart tx
{
	int a[4];
	for(int i=0;i<4;i++)
	{
		a[i]=(c%10)+48;
		c=c/10;
	}
	for(int i=4;i>0;i--)
	{
		UDR1=a[i];
		lcd_data(a[i]);
		if(i==3) 
		{
			UDR1='.';
			lcd_data('.');
		}
		_delay_ms(2);
	}
	
    UDR1='V';
	lcd_data('V');
	UDR1=' ';
	lcd_data(' ');
	
	lcd_cmd(0x88);		//Force cursor to position where vale is to printed on lcd
	
}
int convert_volt()					//function to conv 10 bit adc to voltage*2000
{
	
	float temp;
	int x;
	temp=(adc_val*5120)/1024;
	x=temp; 
	return x;
}

ISR(ADC_vect)
{
	
	
	adc_val=ADC;
	
}

int main(void)
{	
	sei();
	DDRC=0Xff;
	
	lcd_init();
	uart_init();
	adc_init();
	
	lcd_string("ADC Val");
	lcd_cmd(0x88);
	
	
    while (1) 
    {	
		PORTC=adc_val;
		convert_bits(convert_volt());
    }
}

