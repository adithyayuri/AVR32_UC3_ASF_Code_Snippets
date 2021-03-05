#include <asf.h>
#define oscf 16000000

////////////////////////////////////////////////////////////////////////////////////
//  Example description
//  1.Clock is configured to use 16Mhz crystal as main clock.
//  2.Interrupt for usart2 IRQ is enabled in configintc()
//	3.USART Configuration
//		a.baudrate set to 9600 bps
//		b.Character length set to 8
//		c.No parity and 1 stopbits
//	4.USART is initialized using usart_init_rs232()
//	5.RTOR value is set to FFFF and the interrupt on timeout is enabled
//  6.USART Timeout interrupt is enabled
//  7.First a character is received from the user
//  8.Timeout runs in the background
//  9.Keep sending characters to avoid timeout interrupt
// 10.If the timeout happens, the interrupt handler is executed
// 11.Since STTTO bit is set, another char has to be received to rearm the counter
// 12.After receiving a character it rearms and loads the RTOR register
// 13.Goto point number 8
/////////////////////////////////////////////////////////////////////////////////////

#if defined (__GNUC__)
__attribute__((__interrupt__))
#elif defined(__ICCAVR32__)
__interrupt
#endif
void intrupt_handler()
{
	usart_write_line(&AVR32_USART2,"\r\nReciever Timeout occurred - Send a char to reload RTOR");

	AVR32_USART2.cr |= 0x800; //Set STTTO bit
}

void configintc()
{
	Disable_global_interrupt();
	INTC_init_interrupts();
	INTC_register_interrupt((__int_handler) intrupt_handler,AVR32_USART2_IRQ,AVR32_INTC_INT1); //Enable Interrupt on USART2 IRQ
	Enable_global_interrupt();
}

void configusart()
{
	gpio_enable_module_pin(AVR32_PIN_PC04,2); //Enable txd pin
	gpio_enable_module_pin(AVR32_PIN_PC05,2); //Enable rxd pin

	//USART Config
	usart_options_t opt=
	{
		.baudrate = 9600,
		.channelmode = USART_NORMAL_CHMODE,
		.charlength = 8,
		.paritytype = USART_NO_PARITY,
		.stopbits = USART_1_STOPBIT
	};
	usart_init_rs232(&AVR32_USART2,&opt,oscf);
	AVR32_USART2.rtor = 0xFFFF; //Set rtor register
	AVR32_USART2.ier = AVR32_USART_IER_TIMEOUT_MASK; //Enable interrupt on timeout
}

int main (void)
{
	int a=0;

	sysclk_init(); //Using 16MHZ crystal as main clock
	configintc(); //Interrupt config
	configusart(); //USART config

	usart_write_line(&AVR32_USART2,"\r\nExample program with Receiver timeout with STTTO");
	usart_write_line(&AVR32_USART2,"\r\nSend a char to start program");
	while ((AVR32_USART2.csr & AVR32_USART_CSR_RXRDY_MASK) != AVR32_USART_CSR_RXRDY_MASK);
	usart_read_char(&AVR32_USART2,&a);
	usart_reset_status(&AVR32_USART2);
	usart_write_line(&AVR32_USART2,"\r\nNow keep sending char within ~7s OR wait for RTOR interrupt");

	while(1)
	{
		while ((AVR32_USART2.csr & AVR32_USART_CSR_RXRDY_MASK) != AVR32_USART_CSR_RXRDY_MASK);
		usart_write_line(&AVR32_USART2,"\r\n");
		usart_read_char(&AVR32_USART2,&a);
		usart_write_line(&AVR32_USART2,"\r\nChar received");
		usart_reset_status(&AVR32_USART2);
	}
}
