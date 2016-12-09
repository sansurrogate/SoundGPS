#include "bss/rpi_bss.h"
#include "gpio/rpi_gpio.h"
#include "serial/rpi_serial.h"
#include "pwm/rpi_pwm.h"
#include "timer/rpi_timer.h"
#include "interrupt/rpi_interrupt.h"

void rpi_init(void){
	// bss領域をクリア(おまじない)
	bss_clear();

	// まず、割り込みを全て無効化
	interrupt_disable_all_IRQ();

	// システムタイマを初期化
	syst_init();

	// GPIOを安全な状態に設定
	gpio_init();

	// uart通信を115200bpsで開始
	serial_begin(115200);

	// pwmを初期化
	pwm_init();

	// タイマー割り込みの割り込み周期を10000(us)に設定し、初期化(割り込みはまだ無効のまま)
	//タイマー割り込みを有効にするにはtimer_attach_interrupt関数を使う
	timer_init(10000);
}
