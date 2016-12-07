#include "bss/rpi_bss.h"
#include "gpio/rpi_gpio.h"
#include "serial/rpi_serial.h"
#include "timer/rpi_timer.h"
#include "interrupt/rpi_interrupt.h"

void rpi_init(void){
	bss_clear();

	interrupt_disable_all_IRQ();

	// set_vector_table();

	syst_init();

	gpio_init();

	serial_begin(115200);

	timer_init(10000);
}
