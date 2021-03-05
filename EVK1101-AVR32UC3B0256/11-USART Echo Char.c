#include <asf.h>
#define oscf 12000000

void config()
{
	pm_switch_to_osc0(&AVR32_PM,oscf,AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);
	gpio_configure_pin(AVR32_PIN_PA22,GPIO_DIR_OUTPUT | GPIO_INIT_LOW); //LED pin
}

void configusart()
{
	gpio_enable_module_pin(AVR32_PIN_PA23,0); //Enable txd pin
	gpio_enable_module_pin(AVR32_PIN_PA24,0); //Enable rxd pin

	usart_options_t opt=
	{
		.baudrate = 9600,
		.channelmode = USART_NORMAL_CHMODE,
		.charlength = 8,
		.paritytype = USART_NO_PARITY,
		.stopbits = USART_1_STOPBIT
	};

	usart_init_rs232(&AVR32_USART1,&opt,oscf);
}

int main (void)
{
	config();
	configusart();
	usart_write_line(&AVR32_USART1,"Program Started");
	while(1)
	{
		usart_get_echo_line(&AVR32_USART1);
	}
}

