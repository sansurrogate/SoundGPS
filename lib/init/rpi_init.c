#include "bss/rpi_bss.h"
#include "gpio/rpi_gpio.h"
#include "serial/rpi_serial.h"
#include "timer/rpi_timer.h"
#include "interrupt/rpi_interrupt.h"

void rpi_init(void){
	clear_bss();

	disable_all_IRQ();

	set_vector_table();

	init_syst();

	init_gpio();

	Serial_begin(115200);

	Timer_initialize(1000);
}
