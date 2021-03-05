#include <asf.h>

int main (void)
{
	// Enable GPIO pin
	gpio_enable_gpio_pin(AVR32_PIN_PB28);
	// Configure GPIO pin as output
	gpio_configure_pin(AVR32_PIN_PB28,GPIO_DIR_OUTPUT);
	while(1)
	{
		// Set GPIO pin low
		gpio_clr_gpio_pin(AVR32_PIN_PB28);
		for(long int i=0;i<500;i++)
		{
			// Dummy delay
		}
		// Set GPIO pin hish
		gpio_set_gpio_pin(AVR32_PIN_PB28);
		for(long int i=0;i<500;i++)
		{
			// Dummy delay
		}
	}
}
