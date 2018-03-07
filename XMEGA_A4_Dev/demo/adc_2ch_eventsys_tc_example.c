/*


This project demonstrates the basic use of the ATXMEGA ADC.

USE ASF Wizzard to include the following modules:

 - ADC
 - TC

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



#define F_CPU 2000000UL

#include <asf.h>
#include <util/delay.h>

#define MY_ADC    ADCA
#define MY_TIMER  TCC0
static void evsys_init(void)
{
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	EVSYS.CH3MUX = EVSYS_CHMUX_TCC0_OVF_gc;
}
static void tc_init(void)
{
	tc_enable(&MY_TIMER);
	tc_set_wgm(&MY_TIMER, TC_WG_NORMAL);
	tc_write_period(&MY_TIMER, 10000); // count to 10000 than interrupt	(5ms @2MHz system clock)
	//tc_set_resolution(&MY_TIMER, sysclk_get_cpu_hz()/1000); //counter clock 2000 Hz
	tc_write_clock_source(&MY_TIMER, TC_CLKSEL_DIV1_gc);

}


static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, ADC_CH0, &adcch_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,	ADC_REF_VCC);
	
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SWEEP, 2, 3);
	
	adc_enable_internal_input(&adc_conf, ADC_INT_BANDGAP | ADC_INT_TEMPSENSE);
	adc_set_clock_rate(&adc_conf, 200000UL); //200kHz SAMPLING RATE
	
	adc_write_configuration(&MY_ADC, &adc_conf);
	
	adcch_set_input(&adcch_conf, ADCCH_POS_TEMPSENSE, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(&MY_ADC, ADC_CH0, &adcch_conf);
	
	adcch_set_input(&adcch_conf, ADCCH_POS_BANDGAP, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(&MY_ADC, ADC_CH1, &adcch_conf);
}



int main (void)
{
	sysclk_init();
	evsys_init();
	tc_init();
	adc_init();
	ioport_init();
	
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTA,0), IOPORT_DIR_INPUT);
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTA,1), IOPORT_DIR_INPUT);
		
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTA,2), IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTA,3), IOPORT_DIR_OUTPUT);
	
	
	ioport_set_pin_high(IOPORT_CREATE_PIN(PORTA,2));
	_delay_ms(500);
	ioport_set_pin_low(IOPORT_CREATE_PIN(PORTA,2));
	_delay_ms(500);
	ioport_set_pin_high(IOPORT_CREATE_PIN(PORTA,2));
	_delay_ms(500);
	ioport_set_pin_low(IOPORT_CREATE_PIN(PORTA,2));
	_delay_ms(500);
	
	
	
	adc_enable(&MY_ADC);
	

	
	do {
		
		
		ioport_set_pin_low(IOPORT_CREATE_PIN(PORTA,3));
		ioport_set_pin_high(IOPORT_CREATE_PIN(PORTA,3));
		
		uint16_t tmp_result;
		uint16_t bg_result;
		if (adc_get_interrupt_flag(&MY_ADC, ADC_CH0 | ADC_CH1) == (ADC_CH0 | ADC_CH1)) {
		
			ioport_set_pin_high(IOPORT_CREATE_PIN(PORTA,2));	
			
			
			tmp_result = adc_get_result(&MY_ADC, ADC_CH0);
			bg_result = adc_get_result(&MY_ADC, ADC_CH1);
			adc_clear_interrupt_flag(&MY_ADC, ADC_CH0 | ADC_CH1);
			
			ioport_set_pin_low(IOPORT_CREATE_PIN(PORTA,2));
		}
		
	} while (true);
	
}
