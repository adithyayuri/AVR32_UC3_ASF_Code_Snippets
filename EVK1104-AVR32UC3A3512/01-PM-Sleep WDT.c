#include <asf.h>

#define wdt_timeout 1000000

wdt_opt_t opt= {
	.us_timeout_period = wdt_timeout,
	//.cssel = WDT_CLOCK_SOURCE_SELECT_RCSYS,
	//.fcd = false,
	//.swf = false,
	//.mode = WDT_BASIC_MODE,
	//.dar = false
};

int main (void)
{
    uint8_t i;

    gpio_configure_pin(AVR32_PIN_PX51,GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);

    for(i=0;i<4;i++)
    {
        gpio_set_pin_low(AVR32_PIN_PX51);
        cpu_delay_ms(10,115000);
        gpio_set_pin_high(AVR32_PIN_PX51);
        cpu_delay_ms(10,115000);
    }
    wdt_enable(&opt);
    pm_sleep(AVR32_PM_SMODE_DEEP_STOP);
    while(1);
}
