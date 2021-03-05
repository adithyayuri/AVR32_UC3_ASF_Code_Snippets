#include <asf.h>

#define wdt_timeout 4000000
#define oscf 12000000

void led_blink()
{
	uint8_t i;
	for(i=0;i<5;i++)
	{
		gpio_set_pin_low(AVR32_PIN_PB30);
		cpu_delay_ms(100,oscf);
		gpio_set_pin_high(AVR32_PIN_PB30);
		cpu_delay_ms(100,oscf);
	}
}

void config()
{
	pm_switch_to_osc0(&AVR32_PM,oscf,AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);
	gpio_configure_pin(AVR32_PIN_PB30,GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_enable_module_pin(AVR32_PIN_PA20,0);
	gpio_enable_pin_pull_up(AVR32_PIN_PA20);

	eic_options_t opt;
	opt.eic_line = EXT_NMI;
	opt.eic_mode = EIC_MODE_LEVEL_TRIGGERED;
	opt.eic_level = EIC_LEVEL_LOW_LEVEL;
	opt.eic_filter = EIC_FILTER_ENABLED;
	opt.eic_async = EIC_ASYNCH_MODE;

	eic_init(&AVR32_EIC,&opt,1);
	eic_enable_line(&AVR32_EIC,EXT_NMI);
}

int main (void)
{

	config();
	led_blink();

	AVR32_INTC.ipr[0];  // Dummy read
	SLEEP(AVR32_PM_SMODE_STATIC);

	while(1)
	{

		led_blink();
		eic_clear_interrupt_line(&AVR32_EIC,EXT_NMI);
		AVR32_INTC.ipr[0];
		SLEEP(AVR32_PM_SMODE_STATIC);
	}
}
