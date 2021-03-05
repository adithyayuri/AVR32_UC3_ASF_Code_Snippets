#include <asf.h>
#define oscf 12000000

uint8_t adata[100];
uint32_t result;

#if defined (__GNUC__)
__attribute__((__interrupt__))
#elif defined(__ICCAVR32__)
__interrupt
#endif
void pdca_handler()
{
	char buffer0[15];

	for(int i=0;i<100;i++)
	{
		result = (result + adata[i]);
	}
	result = result / 100;

	pdca_reload_channel(0,(void *)adata,sizeof(adata));

	snprintf(buffer0,15,"%d",result);
	usart_write_line(&AVR32_USART1,"\r\n");
	usart_write_line(&AVR32_USART1,buffer0);
}


void config()
{
	pm_switch_to_osc0(&AVR32_PM,oscf,AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC);
	gpio_configure_pin(AVR32_PIN_PA07,GPIO_DIR_OUTPUT | GPIO_INIT_LOW); //LED pin

	//config LED0
	volatile unsigned long *gpers_add = 0xFFFF1004;
	volatile unsigned long *oders_add = 0xFFFF1044;
	volatile unsigned long *ovrt_add = 0xFFFF105C;

	*gpers_add = 1 << (22 & 0x1F);
	*oders_add = 1 << (22 & 0x1F);
	for (uint8_t i=0;i<10;i++)
	{
		*ovrt_add = 1 << (22 & 0x1F);
		cpu_delay_ms(100,oscf);
	}
}

void configusart()
{
	gpio_enable_module_pin(AVR32_PIN_PA23,0); //Enable txd pin
	gpio_enable_module_pin(AVR32_PIN_PA24,0); //Enable rxd pin

	usart_options_t opt=
	{
		.baudrate = 57600,
		.channelmode = USART_NORMAL_CHMODE,
		.charlength = 8,
		.paritytype = USART_NO_PARITY,
		.stopbits = USART_1_STOPBIT
	};

	usart_init_rs232(&AVR32_USART1,&opt,oscf);
}

void configadc()
{
	//CONFIG MODE REGISTER
	AVR32_ADC.mr |= 1 << AVR32_ADC_LOWRES_OFFSET;
	AVR32_ADC.mr |= 0xF << AVR32_ADC_SHTIM_OFFSET;
	AVR32_ADC.mr |= 0x1F << AVR32_ADC_STARTUP_OFFSET;

	adc_enable(&AVR32_ADC,6);
}

void configure_pdc()
{
	 pdca_channel_options_t pdca_opt = {
		 .pid = AVR32_PDCA_PID_ADC_RX,
		 .transfer_size = PDCA_TRANSFER_SIZE_BYTE,
		 .addr = (void *)adata,
		 .size = sizeof(adata),
		 .r_addr = NULL,
		 .r_size = 0,
	 };

	 pdca_init_channel(0,&pdca_opt);
	 pdca_enable(0);
}

void configure_intrupt()
{
	pdca_enable_interrupt_transfer_complete(0);

	INTC_init_interrupts();

	INTC_register_interrupt(pdca_handler,AVR32_PDCA_IRQ_0,AVR32_INTC_INT1);

	Enable_global_interrupt();

}
int main (void)
{

	char a[10];
	uint32_t adcvalue = 0;


	config();
	configusart();
	configadc();
	usart_write_line(&AVR32_USART1,"Program Started");
	configure_pdc();
	configure_intrupt();

	while(1)
	{
		adc_start(&AVR32_ADC);
	}
}

