/*

ASF WIZARD
 USART Driver
 USART Service
 
*/


#define F_CPU 2000000UL
#include <asf.h>
#include <util/delay.h>


#define USART_SERIAL                     &USARTD0
#define USART_SERIAL_BAUDRATE            9600UL
#define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc

#define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT            false



void usart_init(){
	
	 static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		 .baudrate = USART_SERIAL_BAUDRATE,
		 .charlength = USART_SERIAL_CHAR_LENGTH,
		 .paritytype = USART_SERIAL_PARITY,
		 .stopbits = USART_SERIAL_STOP_BIT
	 };
	 sysclk_enable_module(SYSCLK_PORT_D, PR_USART0_bm);
	 usart_init_rs232(USART_SERIAL, &USART_SERIAL_OPTIONS);
	
}

int main (void)
{
	ioport_init();
	sysclk_init();
	//sysclk_set_source(SYSCLK_RTCSRC_EXTCLK);
	board_init();
    sysclk_enable_module(SYSCLK_PORT_D, PR_USART0_bm); 
	usart_init();
	
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTA,2), IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTA,3), IOPORT_DIR_OUTPUT);
	
	while(1){
		
		ioport_set_pin_high(IOPORT_CREATE_PIN(PORTA,2));
		_delay_us(100);
		ioport_set_pin_low(IOPORT_CREATE_PIN(PORTA,2));
		
		if (usart_rx_is_complete(USART_SERIAL)){
			uint16_t received_byte;
			received_byte = usart_getchar(USART_SERIAL);
			usart_putchar(USART_SERIAL, '-');
			usart_putchar(USART_SERIAL, received_byte);
			
			usart_clear_rx_complete (USART_SERIAL);	
		}
		
     	
	usart_putchar(USART_SERIAL, '-');
		
	}
}
