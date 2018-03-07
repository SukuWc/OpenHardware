/*

This project demonstrates the basic use of the ATXMEGA 12 bit DAC.

USE ASF Wizzard to include the following modules:

 - DAC
 - TC

A Low resolution sine-wave is output on PB2 at a sample rate of 22050Hz.
The lenght of the wavetable is 32 samples, so the resulting waveform has
a fundamental frequency of 689Hz
 
SUKU 2017.11.30.

*/



#define SPEAKER_DAC              DACB
#define SPEAKER_DAC_CHANNEL      DAC_CH0
#define RATE_OF_CONVERSION       22050
#define NR_OF_SAMPLES            32

#include <asf.h>


static const uint16_t sine[NR_OF_SAMPLES] = {
	32768, 35325, 37784, 40050, 42036, 43666, 44877, 45623,
	45875, 45623, 44877, 43666, 42036, 40050, 37784, 35325,
	32768, 30211, 27752, 25486, 23500, 21870, 20659, 19913,
	19661, 19913, 20659, 21870, 23500, 25486, 27752, 30211,
};

static void evsys_init(void)
{
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);
	EVSYS.CH3MUX = EVSYS_CHMUX_TCC0_OVF_gc;
}

void tc_init(void)
{
	tc_enable(&TCC0);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, (sysclk_get_per_hz() / RATE_OF_CONVERSION) - 1);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1_gc);
}

void dac_init(void)
{
	struct dac_config conf;
	dac_read_configuration(&SPEAKER_DAC, &conf);
	dac_set_conversion_parameters(&conf, DAC_REF_BANDGAP, DAC_ADJ_LEFT);
	dac_set_active_channel(&conf, SPEAKER_DAC_CHANNEL, 0);
	dac_set_conversion_trigger(&conf, SPEAKER_DAC_CHANNEL, 3);
	#ifdef XMEGA_DAC_VERSION_1
	dac_set_conversion_interval(&conf, 1);
	#endif
	dac_write_configuration(&SPEAKER_DAC, &conf);
	dac_enable(&SPEAKER_DAC);
}

int main (void)
{
	
	sysclk_init();
	evsys_init();
	tc_init();
	dac_init();

	board_init();

    uint8_t i = 0;
	while(1){
		
		dac_wait_for_channel_ready(&SPEAKER_DAC, SPEAKER_DAC_CHANNEL);
		dac_set_channel_value(&SPEAKER_DAC, SPEAKER_DAC_CHANNEL, sine[i]);
		i++;
		i %= NR_OF_SAMPLES;
		
		
	}
	/* Insert application code here, after the board has been initialized. */
}
