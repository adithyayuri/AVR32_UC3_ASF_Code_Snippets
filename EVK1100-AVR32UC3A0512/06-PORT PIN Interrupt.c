#include <asf.h>

#if defined (__GNUC__)
__attribute__((__interrupt__))
#elif defined(__ICCAVR32__)
__interrupt
#endif
void intrupt_handler()
{
 	for (int i=51;i<=54;i++)
 	{
		gpio_set_pin_low(i);

 		for(long int i=0;i<1000;i++);

 		gpio_set_pin_high(i);

 		for(long int i=0;i<1000;i++);

 	}
	gpio_clear_pin_interrupt_flag(AVR32_PIN_PX19);
}

void board_conf_gpio()
{
	gpio_enable_gpio_pin(AVR32_PIN_PB27);
	gpio_configure_pin(59,GPIO_DIR_OUTPUT);

	gpio_enable_gpio_pin(60);
	gpio_configure_pin(60,GPIO_DIR_OUTPUT);

	gpio_enable_gpio_pin(61);
	gpio_configure_pin(61,GPIO_DIR_OUTPUT);

	gpio_enable_gpio_pin(62);
	gpio_configure_pin(62,GPIO_DIR_OUTPUT);

	gpio_enable_gpio_pin(51);
	gpio_configure_pin(51,GPIO_DIR_OUTPUT);

	gpio_enable_gpio_pin(52);
	gpio_configure_pin(52,GPIO_DIR_OUTPUT);

	gpio_enable_gpio_pin(53);
	gpio_configure_pin(53,GPIO_DIR_OUTPUT);

	gpio_enable_gpio_pin(54);
	gpio_configure_pin(54,GPIO_DIR_OUTPUT);

	//GPIO INTRUPT CONFIG
	gpio_enable_pin_interrupt(AVR32_PIN_PX19 , GPIO_PIN_CHANGE);
	Disable_global_interrupt();
	INTC_init_interrupts();
	INTC_register_interrupt( (__int_handler) intrupt_handler,AVR32_GPIO_IRQ_10, AVR32_INTC_INT0);

	Enable_global_interrupt ();
}

int main (void)
{
	board_conf_gpio();
	while(1)
	{
        for (int i=59;i<=62;i++)
        {
            gpio_set_pin_low(i);

            for(long int i=0;i<1000;i++);

            gpio_set_pin_high(i);

            for(long int i=0;i<1000;i++);
        }
	}
}



