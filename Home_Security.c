#define F_CPU 1000000UL
#include<avr/io.h>
#include<string.h>
#include<inttypes.h>
#include<util/delay.h>
#include"lcd.h"
#include"SMS.h"
#include"USART.h"
#include "internal_adc.h"

#define TIME  				1000
#define WARM_UP				5
#define JTAG_DISABLE()		{MCUCSR|=0x80;MCUCSR|=0x80;}	//JTAG disabled
#define BUZZER_ON			{PORTC |= 0x08;}
#define BUZZER_OFF			{PORTC &= ~0x08;}
#define JTAG_DISABLED  		MCUCSR = 0x80;MCUCSR = 0x80;    //Disable JTAG Port So that Port can be used as I/O
#define THRESHOLD_CO		1000
#define THRESHOLD_LPG       1000   
#define THRESHOLD_TEMP      40


unsigned int convert_camo (unsigned int camo);
unsigned int convert_lpg (unsigned int );

void warm_up (void);
void hw_init();

unsigned char mes[16];


void warm_up (void)
{
	static unsigned char count = 0;
	lcd_string ("Warming up...", 1);
	PORTC |= 0x01;
	while (count < WARM_UP)									//warm up time
	{
		sprintf (mes, "%d seconds", count++);
		lcd_string (mes, 2);
		BUZZER_ON
		_delay_ms (200);
		BUZZER_OFF
		_delay_ms (800);
	}
	PORTC &= ~0x01;
	sprintf (mes, "Warm up complete");
	lcd_string (mes, 2);

}
void hw_init()
{	
	DDRC = 0xFF;
	JTAG_DISABLED;
	lcd_init();                     
	USART_Init(12);
	gsm_init();
	warm_up();	
	lcd_clr_line (1);
	lcd_clr_line (2);
	lcd_string ("Home Security",1);
	lcd_string ("System",2);
	_delay_ms(2000);

}
void lcd_blink(void)
{
	unsigned char i;
	for(i=0;i<4;i++)
	{
		lcd_clr_line(1);
		lcd_clr_line(2);
		BUZZER_ON
		_delay_ms(500);
		BUZZER_OFF
		lcd_string("Warning!!!!!!!",1);
		_delay_ms(TIME);
		lcd_clr_line(1);
		lcd_clr_line(2);
	}
}
unsigned int convert_lpg (unsigned int lpg_volt)
{
	static unsigned int temp=0;
	temp = lpg_volt = (float)lpg_volt * 4.8875;    //Calibration for LPG sensor.2500mv = (2500*2)ppm =5000 ppm
	return temp;
}


unsigned int convert_camo (unsigned int camo)
{
	static unsigned int temp=0;
	temp = (unsigned int)((float)(camo) * 3.33);
	return temp;
}



int main(void)
{
	int result;
	unsigned char sent[19],z;
	
	static unsigned char i,j;
	static unsigned int camo,lpg_value,lpg_volt;
	unsigned char temperature;
	static unsigned char buffer[16];
	static unsigned char sensor_ascii[4];
	hw_init();
	lcd_clr_line(1);
	lcd_clr_line(2);
	adc_init();
	while(1)
	{
		adc_chan0 ();
		temperature =  adc_scan ();
		if (i == 0)
		{
			adc_chan1 ();
			camo = adc_scan ();
			i++;
		}
		else
		{
			adc_chan1 ();
			camo += adc_scan ();
			camo /= 2;
			i++;	
		}
		if (i == 100)
		{
			i = 0;
			camo = convert_camo (camo);
			sprintf (buffer,"CO = %d ppm    ", camo);
			lcd_string (buffer,2);
		}

		/* Scannig for LPG gas */
		
		if (j == 0)
		{
			adc_chan2 ();
			lpg_volt = adc_scan ();
			j++;
		}
		else
		{
			adc_chan2 ();
			lpg_volt += adc_scan ();
			lpg_volt /= 2;
			j++;	
		}
		if (j == 100)
		{
			j = 0;
			lpg_volt = convert_lpg (lpg_volt);
			lpg_value = lpg_volt*2 ;
		}
	
		sprintf(sensor_ascii,"T=%d LPG=%dppm ",temperature/2,lpg_value);
		lcd_string(sensor_ascii,1);	

		if(temperature/2 >THRESHOLD_TEMP || camo > THRESHOLD_CO || (lpg_value) > THRESHOLD_LPG)
		{
			
			lcd_blink();
			lcd_string("Sending SMS",1);
			sms_1();
			USART_Txstr("Warning!!!!!!! Bulding is on fire");     	//message to be transmitted
		//	_delay_ms(200);
			USART_Transmit(0x1A);             						//Ctrl+z for sending msg
			lcd_string("SMS Sent",1);
		}
	}
}

				 
