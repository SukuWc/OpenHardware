/*



*/

#define F_CPU 2000000UL

#include <asf.h>
#include <util/delay.h>

#define MY_ADC    ADCA
#define MY_TIMER  TCC0

#define MY_DAC    DACB
#define MY_DAC_0  DAC_CH0
#define MY_DAC_1  DAC_CH1

static const uint16_t sine[16] = {
	0, 4095,	0, 4095,	0, 4095,	0, 4095,	0, 4095,	0, 4095,	0, 4095,	0, 4095
};

static void evsys_init(void)
{
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	EVSYS.CH3MUX = EVSYS_CHMUX_TCC0_OVF_gc;
}
static void tc_init(void)
{
	tc_enable(&MY_TIMER);
	tc_set_wgm(&MY_TIMER, TC_WG_NORMAL);
	tc_write_period(&MY_TIMER, 100); // count to 10000 than interrupt	(5ms @2MHz system clock)
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



	//adcch_set_input(&adcch_conf, ADCCH_POS_TEMPSENSE, ADCCH_NEG_NONE, 1);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(&MY_ADC, ADC_CH0, &adcch_conf);
	
	//adcch_set_input(&adcch_conf, ADCCH_POS_BANDGAP, ADCCH_NEG_NONE, 1);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(&MY_ADC, ADC_CH1, &adcch_conf);
}

void dac_init(void)
{
	struct dac_config conf;
	dac_read_configuration(&MY_DAC, &conf);
	dac_set_conversion_parameters(&conf, DAC_REF_AVCC, DAC_ADJ_RIGHT);
	dac_set_active_channel(&conf, MY_DAC_0 | MY_DAC_1, 0);
	dac_set_conversion_trigger(&conf, MY_DAC_0 | MY_DAC_1, 3);
	#ifdef XMEGA_DAC_VERSION_1
	
	dac_set_conversion_interval(&conf, 2);
	#endif
	dac_write_configuration(&MY_DAC, &conf);
	dac_enable(&MY_DAC);
}



int main (void)
{
	sysclk_init();
	evsys_init();
	tc_init();
	adc_init();
	dac_init();
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
	

	uint8_t i = 0;
	 
	do {
		
		
		ioport_set_pin_low(IOPORT_CREATE_PIN(PORTA,3));
		ioport_set_pin_high(IOPORT_CREATE_PIN(PORTA,3));
		
		uint16_t an1;
		uint16_t an2;
		if (adc_get_interrupt_flag(&MY_ADC, ADC_CH0 | ADC_CH1) == (ADC_CH0 | ADC_CH1)) {
		
			ioport_set_pin_high(IOPORT_CREATE_PIN(PORTA,2));	
			
			
			an1 = adc_get_result(&MY_ADC, ADC_CH0);
			an2 = adc_get_result(&MY_ADC, ADC_CH1);
			adc_clear_interrupt_flag(&MY_ADC, ADC_CH0 | ADC_CH1);
			
			
			//dac_wait_for_channel_ready(&MY_DAC, MY_DAC_0);
			dac_set_channel_value(&MY_DAC, MY_DAC_0, an1);
			//dac_wait_for_channel_ready(&MY_DAC, MY_DAC_1);
			dac_set_channel_value(&MY_DAC, MY_DAC_1, 4095-an1);
			i++;
			i %= 16;
			
			
			ioport_set_pin_low(IOPORT_CREATE_PIN(PORTA,2));
		}
	
		
	} while (true);
	
}
