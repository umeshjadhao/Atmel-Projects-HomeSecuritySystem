/************************************************************************
AT Commands
AT---------------->Attention Command
AT+CMGF=1--------->Mode Set
AT+CSCS="GSM"----->GSM Mode Select


*************************************************************************/

#include<avr/io.h>
#include<util/delay.h>
#include"SMS.h"

//unsigned char ack1[]="1310OK1310";
unsigned char rec[10];


void gsm_init()                                       // Function to initialize the GSM module
{
//	unsigned char rec[6];
	lcd_string("Initializing...",1);
	USART_Txstr("AT");
	_delay_ms(1000);
	USART_Transmit(0x0D);
	_delay_ms(1000);
	USART_Txstr("AT+CMGF=1");
	_delay_ms(200);
	USART_Transmit(0x0D);
	_delay_ms(1000);	
	lcd_string("Initialized");	
	_delay_ms(1000);
}
void sms_1()
{
	
//	unsigned char m;
	USART_Txstr("AT+CMGS=");
	USART_Transmit('"');
	USART_Txstr("+919762175256");
	USART_Transmit('"');
	USART_Transmit(0x0D);
}

