#include <asf.h>

int main (void)
{
	// Enable crystal
	pm_enable_osc0_crystal(&AVR32_PM, AVR32_PM_OSCCTRL0_STARTUP_4096_RCOSC);
	pm_enable_clk0(&AVR32_PM, AVR32_PM_OSCCTRL0_STARTUP_4096_RCOSC);

	// Setup PLL
	pm_pll_set_option(&AVR32_PM,0,0,0,0);
	pm_pll_setup(&AVR32_PM,0,14,1,0,16);

	// Enable PLL
	pm_pll_enable(&AVR32_PM,0);
	pm_wait_for_pll0_locked(&AVR32_PM);

	// Switch to PLL clock
	pm_cksel(&AVR32_PM,0,0,0,0,1,1);
	// Setup wait states
	flashc_set_wait_state(1);
	pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCSEL_PLL0);

	// MUX pin and enable gclock
	gpio_enable_module_pin(AVR32_PIN_PA03,AVR32_PM_GCLK_0_FUNCTION);
	pm_gc_setup(&AVR32_PM,0,1,0,1,5);
	pm_gc_enable(&AVR32_PM,0);
}
