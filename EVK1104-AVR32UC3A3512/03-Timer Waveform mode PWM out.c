#include <asf.h>

#define wdt_timeout 4000000
#define oscf 12000000

void led_blink();

/////////////////////////
// Probe PB05 and PB06 //
////////////////////////

void config()
{
	pm_switch_to_osc0(&AVR32_PM,oscf,AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);
	gpio_configure_pin(AVR32_PIN_PX51,GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_enable_module_pin(AVR32_PIN_PB05,1); //case- 1 - b function
	gpio_enable_module_pin(AVR32_PIN_PB06,1); //case- 1 - b function

	//Waveform option selection
	tc_waveform_opt_t opt=
	{
		.channel = 0,
		.aswtrg = TC_EVT_EFFECT_NOOP, //Soft trigger in ti0a
		.aeevt = TC_EVT_EFFECT_NOOP, //Ext event in ti0a
		.acpc = TC_EVT_EFFECT_TOGGLE, //RC comp effect on ti0a
		.acpa = TC_EVT_EFFECT_TOGGLE, //Ra comp effect on ti0a

		.bswtrg = TC_EVT_EFFECT_NOOP, //Soft trigger in ti0b
		.beevt = TC_EVT_EFFECT_NOOP, //Ext event in ti0b
		.bcpc = TC_EVT_EFFECT_TOGGLE, //RC comp effect on ti0b
		.bcpb = TC_EVT_EFFECT_TOGGLE, //Ra comp effect on ti0b

		.wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER, //mode 2
		.enetrg = false, //ext trigger enable
		.eevt = 1, //external event selection
		.eevtedg = false, //ext event edge
		.cpcdis = false, //counter disable when rc compare
		.cpcstop = false, //counter stop when rc compare

		.burst = TC_BURST_NOT_GATED, //Burst signal selection
		.clki = TC_CLOCK_RISING_EDGE, //Clock edge select - clock inversion
		.tcclks = TC_CLOCK_SOURCE_TC2, // internal source 2 .... connected to fpba/2
	};
	tc_init_waveform(&AVR32_TC0,&opt);  // Initialize the timer/counter waveform.
	tc_start(&AVR32_TC0,0);
}

void led_blink()
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		gpio_set_pin_low(AVR32_PIN_PX51);
		cpu_delay_ms(300,oscf);
		gpio_set_pin_high(AVR32_PIN_PX51);
		cpu_delay_ms(300,oscf);
	}
}

int main (void)
{
    config();
    tc_write_ra(&AVR32_TC0,0,0x0000);     // Set RA value.
    tc_write_rb(&AVR32_TC0,0,0x1000);     // Set RB value.
    tc_write_rc(&AVR32_TC0,0,0x2000);     // Set RC value.
    int j=0x2000;
    gpio_set_pin_low(AVR32_PIN_PX51);

	while(1)
	{
		for (int i=0x00;i<0x2000;i++)
		{
			tc_write_ra(&AVR32_TC0,0,i);     // Set RA value.
			tc_write_rb(&AVR32_TC0,0,j);
			j--;
			cpu_delay_us(1000,oscf);
		}
	     j=0x2000;
	}
}

