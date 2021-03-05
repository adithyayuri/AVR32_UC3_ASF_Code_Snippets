#include <asf.h>

// Accessing GPIOs through local bus.
// Through Local Bus single cycle writes and reads can be performed on port registers.

int main (void)
{
    // Initilize local bus
	gpio_local_init();
    // Set the pin as output
	gpio_local_enable_pin_output_driver(AVR32_PIN_PB28);
	while(1)
	{
        // Supaaahh fast PORT pin toggle
		gpio_local_tgl_gpio_pin(AVR32_PIN_PB28);
	}
}
