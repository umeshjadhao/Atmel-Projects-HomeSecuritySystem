#include<avr/io.h>
#include"USART.h"
#include<util/delay.h>


void USART_Init( unsigned int ubrr)
{

	UCSRA |=(1<<U2X);
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;

	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);

	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);

}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;

}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	{}
	/* Get and return received data from buffer */
	return UDR;
}

void USART_Txstr (char *s) 
{
	//  loop until *s != NULL
	while (*s) 
	{
		USART_Transmit(*s);
		_delay_ms(200);
		s++;
	}
}
