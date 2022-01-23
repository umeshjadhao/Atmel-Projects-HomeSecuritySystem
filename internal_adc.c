#ifndef INTERNAL_ADC_C
#define INTERNAL_ADC_C

#include "internal_adc.h"
#include <avr\io.h>

void adc_init(void)
{
	adc_chan0();	//Aref,internal turned off,right adjust,channel 0 selected
	ADCSRA=0x86;//single conversion with conversion disabled ps=64
}

void adc_chan0(void)
{
	ADMUX&=0x00;	//select adc0 channel
	ADCSRA=0xC6;
}

void adc_chan1(void)
{
	ADMUX|=0x01;	//select adc1 channel
	ADCSRA=0xC6;
}

void adc_chan2(void)
{
	ADMUX&=0x00;	//select adc2 channel
	ADMUX|=0x02;
	ADCSRA=0xC6;
}

unsigned int adc_scan(void)			
{
	while(!bit_is_set(ADCSRA,4));
	ADCSRA|=(1<<ADIF);
	return ADC;
}

#endif
