/*

This project demonstrates the basic use of the ATXMEGA ADC.

USE ASF Wizzard to include the following modules:

 - ADC

Analog voltage read on PA0 controls the frequency of the
square-wave output on PB1. Note that the analog reference is set
to VCC/2, so do not connect voltages over 1.65V to the analog input.

When using potentiometer connect series resistor to reduce voltage

    POTENTIOMETER  SERIES-RESISTOR

        VR=10k   (VCC/2)     R=10k    VCC
   ____/\/\/\/\_____|______/\/\/\/\____|
  |        |
 GND    AnalogIN


 
SUKU 2017.11.30.

*/

#include <asf.h>
#include "sysclk.h"
#include <util/delay.h>

#define MY_ADC    ADCA
#define MY_ADC_CH ADC_CH0

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;

	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);

	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,
	ADC_REF_VCCDIV2 );
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
 
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
	adc_write_configuration(&MY_ADC, &adc_conf);
	
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	adc_init();
	board_init();
	
	PORTB.DIRSET = 0b00000010 ; 
	/* Insert application code here, after the board has been initialized. */
	
	PORTB.OUTSET = 0b00000010 ; // set the output high.
	_delay_ms( 500) ; // wait.
	PORTB.OUTCLR = 0b00000010 ; // set the output low.
	_delay_ms( 500 ) ; // wait.	PORTA.OUTSET = 0b00000010 ;
	
	PORTB.OUTSET = 0b00000010 ; // set the output high.
	_delay_ms( 500 ) ; // wait.
	PORTB.OUTCLR = 0b00000010 ; // set the output low.
	_delay_ms( 500 ) ; // wait.
	
	ioport_init();
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTA,0), IOPORT_DIR_INPUT);
	adc_enable(&MY_ADC);
	while(1){
		
	    uint16_t result;
	    uint16_t result2;
		adc_start_conversion(&MY_ADC, MY_ADC_CH);
		adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);

		result = adc_get_result(&MY_ADC, MY_ADC_CH);
		result2 = result;

		
		PORTB.OUTSET = 0b00000010 ; // set the output high.
		while(result>0){
			_delay_us( 1 ) ; // wait.
			result--;
		}
		
		PORTB.OUTCLR = 0b00000010 ; // set the output low.
		while(result2>0){
			_delay_us( 1 ) ; // wait.
			result2--;
		}
		
	}
}
