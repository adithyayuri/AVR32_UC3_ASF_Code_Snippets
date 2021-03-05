#include <asf.h>

int main (void)
{
    // Start OSC0
	pm_enable_osc0_crystal(&AVR32_PM, AVR32_PM_OSCCTRL0_STARTUP_4096_RCOSC);
	pm_enable_clk0(&AVR32_PM, AVR32_PM_OSCCTRL0_STARTUP_4096_RCOSC);

    // Setup PLL
	pm_pll_set_option(&AVR32_PM,0,0,0,0);
	pm_pll_setup(&AVR32_PM,0,14,1,0,16);

    // Enable PLL
	pm_pll_enable(&AVR32_PM,0);
	pm_wait_for_pll0_locked(&AVR32_PM);

    // Switch to PLL putput
	pm_cksel(&AVR32_PM,0,0,0,0,1,1);
	flashc_set_wait_state(1);
	pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCSEL_PLL0);

    // Initialize local port
	gpio_local_init();
	AVR32_GPIO_LOCAL.port[1].oders = 1 << (28);

    // MUX pin to GCLK function
	gpio_enable_module_pin(AVR32_PIN_PB19,AVR32_PM_GCLK_0_0_FUNCTION);
	pm_gc_setup(&AVR32_PM,0,1,0,1,2);
	pm_gc_enable(&AVR32_PM,0);

    while(1)
    {
        AVR32_GPIO_LOCAL.port[1].ovrt = 1 << (28);
    }
}
