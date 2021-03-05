#include <asf.h>
#define oscf 12000000

void config()
{
	pm_switch_to_osc0(&AVR32_PM,oscf,AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);
	gpio_configure_pin(AVR32_PIN_PA22,GPIO_DIR_OUTPUT | GPIO_INIT_LOW); //LED pin
}

void configpwm()
{
	gpio_enable_module_pin(AVR32_PIN_PA07,0); //Pwm channel 0
	pwm_opt_t popt =
	{
		.diva = AVR32_PWM_DIVA_CLK_OFF,
		.divb = AVR32_PWM_DIVA_CLK_OFF,
		.prea = AVR32_PWM_PREA_MCK,
		.preb = AVR32_PWM_PREA_MCK,
	};

	avr32_pwm_channel_t	pwm0 =
	{
		.ccnt = 0,
		.cdty = 1500,
		.cprd = 2000,
		.cupd = 0
	};
	pwm0.CMR.calg = PWM_MODE_LEFT_ALIGNED;
	pwm0.CMR.cpol = PWM_POLARITY_LOW;
	pwm0.CMR.cpd = PWM_UPDATE_DUTY;
	pwm0.CMR.cpre = AVR32_PWM_CPRE_MCK;

	pwm_init(&popt);
	pwm_channel_init(0,&pwm0);
	pwm_start_channels(1<<0);
}

int main (void)
{
	config();
	configpwm();
	while(1)
	{

	}
}

