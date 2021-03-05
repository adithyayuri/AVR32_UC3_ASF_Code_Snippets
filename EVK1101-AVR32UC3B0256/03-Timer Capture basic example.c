#include <asf.h>

#define oscf 12000000

/* generic clock[1] on pin53 Pb11 Function C */
void uart_config()
{
	gpio_enable_module_pin(AVR32_PIN_PA23,0); //Enable txd pin
	gpio_enable_module_pin(AVR32_PIN_PA24,0); //Enable rxd pin

	static const usart_options_t opt_uart=
	{
		.baudrate     = 57600,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};
	usart_init_rs232(&AVR32_USART1,&opt_uart,oscf);
	usart_write_line(&AVR32_USART1,"USART INITIALIZED \n");
}

void config()
{
	pm_switch_to_osc0(&AVR32_PM,oscf,AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);
	gpio_configure_pin(AVR32_PIN_PA22,GPIO_DIR_OUTPUT | GPIO_INIT_HIGH); //LED pin
	gpio_enable_module_pin(AVR32_PIN_PA26,2); //case C - a0 function TC
	gpio_enable_module_pin(AVR32_PIN_PA27,2); //case C- b0 function TC

	gpio_enable_module_pin(AVR32_PIN_PB02,1); //case b - a1 function TC
	gpio_enable_module_pin(AVR32_PIN_PB03,1); //case b- b1 function TC


	//gclock setup
	gpio_enable_module_pin(AVR32_PIN_PA03,1); //case B - function Gclock
	pm_gc_setup(&AVR32_PM,0,0,0,1,59); //set gclock o/p as 375khz
	pm_gc_enable(&AVR32_PM,0); // enable gclock 0

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

	tc_capture_opt_t cap_opt1=
	{
		.channel = 1, //connect gclock on channel 1 tioa

		.ldra = TC_SEL_RISING_EDGE, //RA rising edge load
		.ldrb = TC_SEL_NO_EDGE, //RB Falling edge load

		.cpctrg = TC_NO_TRIGGER_COMPARE_RC, //dont trigger on rc compare
		.abetrg = TC_EXT_TRIG_SEL_TIOA, //sel a as ext trigger
		.etrgedg = TC_SEL_FALLING_EDGE, //EXT TRG on falling edge

		.ldbdis = false, //counter clock disable with RB loading
		.ldbstop = false, //counter clock stop with rb loading

		.burst = TC_BURST_NOT_GATED,
		.clki = TC_CLOCK_RISING_EDGE,
		.tcclks = TC_CLOCK_SOURCE_TC2 // source 3 ,,, fpba/2
	};
	tc_init_capture(&AVR32_TC, &cap_opt1);
}

int main (void)
{
	char buffer[15];
	uint16_t ra=0;
	char buffer1[15];
	uint16_t fa=0;

	config();
	uart_config();

	tc_start(&AVR32_TC,0);
	usart_write_line(&AVR32_USART1,"Entering While 1");
	while(1)
	{
		ra = tc_read_ra(&AVR32_TC,0);
		fa = tc_read_ra(&AVR32_TC,1);

		snprintf(buffer,15,"%d",ra);
		usart_write_line(&AVR32_USART1,buffer);
		usart_write_line(&AVR32_USART1,"\r \n");

		snprintf(buffer1,15,"%d",fa);
		usart_write_line(&AVR32_USART1,buffer1);
		usart_write_line(&AVR32_USART1,"\r \n \t");
	}
}

