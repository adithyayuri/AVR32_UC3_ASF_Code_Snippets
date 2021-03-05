#include <asf.h>

#define wdt_timeout 4000000
#define oscf 12000000

/* generic clock[1] on pin53 Pb11 Function C */

void config()
{
	pm_switch_to_osc0(&AVR32_PM,oscf,AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);
	gpio_configure_pin(AVR32_PIN_PA22,GPIO_DIR_OUTPUT | GPIO_INIT_HIGH); //LED pin
	gpio_enable_module_pin(AVR32_PIN_PA26,2); //case C - a0 function TC
	gpio_enable_module_pin(AVR32_PIN_PA27,2); //case C- b0 function TC


	//gclock setup
	gpio_enable_module_pin(AVR32_PIN_PA04,1); //case B - function Gclock
	pm_gc_setup(&AVR32_PM,1,0,0,1,199); //set gclock o/p as 375khz
	pm_gc_enable(&AVR32_PM,1); // enable gclock 1


	//Capture settings setup
	tc_capture_opt_t cap_opt=
	{
		.channel = 0, //connect gclock on channel 1 tioa

		.ldra = TC_SEL_FALLING_EDGE, //RA rising edge load
		.ldrb = TC_SEL_NO_EDGE, //RB Falling edge load

		.cpctrg = TC_NO_TRIGGER_COMPARE_RC, //dont trigger on rc compare
		.abetrg = TC_EXT_TRIG_SEL_TIOA, //sel a as ext trigger
		.etrgedg = TC_SEL_RISING_EDGE, //EXT TRG on falling edge

		.ldbdis = false, //counter clock disable with RB loading
		.ldbstop = false, //counter clock stop with rb loading

		.burst = TC_BURST_NOT_GATED,
		.clki = TC_CLOCK_RISING_EDGE,
		.tcclks = TC_CLOCK_SOURCE_TC2 // source 3 ,,, fpba/2
	};
	tc_init_capture(&AVR32_TC, &cap_opt);
}

int main (void)
{
	uint32_t ra;
	config();

	tc_start(&AVR32_TC,0);

	while(1)
	{
		ra = tc_read_ra(&AVR32_TC,1);
	}
}

