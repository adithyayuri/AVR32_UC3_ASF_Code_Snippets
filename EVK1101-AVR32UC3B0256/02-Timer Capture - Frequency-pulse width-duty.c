#include <asf.h>

#define oscf 12000000

void uart_config();
void config();
void config_capture2();

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
	gpio_configure_pin(AVR32_PIN_PA22,GPIO_DIR_OUTPUT | GPIO_INIT_LOW); //LED pin
	gpio_enable_module_pin(AVR32_PIN_PA26,2); //case C - a0 function TC
	gpio_enable_module_pin(AVR32_PIN_PA27,2); //case C- b0 function TC

	//gclock setup
	gpio_enable_module_pin(AVR32_PIN_PA03,1); //case B - function Gclock
	pm_gc_setup(&AVR32_PM,0,0,0,1,59); //set gclock o/p
	pm_gc_enable(&AVR32_PM,0); // enable gclock 0

	gpio_enable_module_pin(AVR32_PIN_PA04,1); //case B - function Gclock
	pm_gc_setup(&AVR32_PM,1,0,0,1,199); //set gclock o/p
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

config_capture2()
{
	gpio_enable_module_pin(AVR32_PIN_PB10,1); //case b - a2 function TC
	gpio_enable_module_pin(AVR32_PIN_PB11,1); //case b- b2 function TC

		//Capture settings setup
		tc_capture_opt_t cap2_opt=
		{
			.channel = 2,

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
		tc_init_capture(&AVR32_TC, &cap2_opt);
}

int main (void)
{
	char buffer0[15];
	char buffer1[15];
	char buffer2[15];
	uint16_t ra=0;
	uint16_t fa=0;
	float pos_duty=0;
	float freq=0;
	float period;

	config();
	config_capture2();
	uart_config();

	tc_start(&AVR32_TC,0); // START TIMER 0
	tc_start(&AVR32_TC,2); // START TIMER 2
	usart_write_line(&AVR32_USART1,"Entering While 1");
	while(1)
	{
		ra = tc_read_ra(&AVR32_TC,0);
		fa = tc_read_ra(&AVR32_TC,2);

		freq = (6/((ra+fa)*(0.001)));
		pos_duty = ((ra*100)/(ra+fa));
		period = (((ra+fa)*(0.001))/6);

		snprintf(buffer0,15,"%f",freq);
		usart_write_line(&AVR32_USART1,"\r\n\nFrequency- ");
		usart_write_line(&AVR32_USART1,buffer0);
		usart_write_line(&AVR32_USART1," KHz");

		snprintf(buffer1,15,"%f",pos_duty);
		usart_write_line(&AVR32_USART1,"    Duty Cycle- ");
		usart_write_line(&AVR32_USART1,buffer1);
		usart_write_line(&AVR32_USART1," %");

		snprintf(buffer1,15,"%f",period);
		usart_write_line(&AVR32_USART1,"    Period- ");
		usart_write_line(&AVR32_USART1,buffer1);
		usart_write_line(&AVR32_USART1," ms");
		//snprintf(buffer0,15,"%d",ra);
		//snprintf(buffer1,15,"%d",fa);
			//
		//usart_write_line(&AVR32_USART1,"\r\nPOS CLK - ");
		//usart_write_line(&AVR32_USART1,buffer0);
		//
		//usart_write_line(&AVR32_USART1,"\tNEG CLK - ");
		//usart_write_line(&AVR32_USART1,buffer1);

	}
}

