#include "timer/rpi_timer.h"
#include "periphs/rpi_periphs.h"
#include <stdio.h>
#include <stdint.h>

#include "gpio/rpi_gpio.h"

extern void enable_IRQ(void);
extern void disable_IRQ(void);
extern unsigned int getmode(void);

// RPI2より前のボードは例外ベクタの位置が異なる
#ifndef RPI2
void interrupt_init_exception_vector(void){
	extern void *_exception_vector_start;
	extern void *_exception_vector_end;
	// volatileをつけないと最適化に消される（涙目）
	volatile unsigned int *vec = 0;
	volatile unsigned int *p;
	volatile unsigned int *s = (unsigned int *)&_exception_vector_start;
	volatile unsigned int *e = (unsigned int *)&_exception_vector_end;

	for (p = s; p < e; p++) {
		*vec = *p;
		vec++;
	}
}
#endif


void interrupt_disable_all_IRQ(void){
	// すべての割り込み不許可
	*INTERRUPT_DISABLE_BASIC_IRQS = 0xffffffff;
	*INTERRUPT_DISABLE_IRQS1 = 0xffffffff;
	*INTERRUPT_DISABLE_IRQS2 = 0xffffffff;
	*INTERRUPT_FIQ_CTRL = 0;

	// IRQ不許可
	disable_IRQ();
}


// IRQ割り込みハンドラ
void interrupt_IRQ_handler(void){
	// IRQ割り込みを停止
	disable_IRQ();
	// printf("0x%04x\r\n",getmode());

	// printf("get IRQ in IRQ handler\r\n");
	// printf("CPSR (in IRQ_handler) = 0x%08x\r\n",getmode());

	static int state = 0;
	static int count = 0;
	if((*INTERRUPT_IRQ_PENDING2 & (1 << 20)) != 0) {
		int flag = (*INTERRUPT_IRQ_PENDING2 >> 17) & (0xf);
		int eds0 = *GPIO_EDS0;
		int eds1 = *GPIO_EDS1;
		printf("gpio_%d 0x%x\r\n", count, flag);
		printf("eds0: 0x%08x\r\n", eds0);
		printf("eds1: 0x%08x\r\n\n", eds1);
		gpio_write(47, state);
		state ^= 1;
		*GPIO_EDS0 = 0xffffffff;
		*GPIO_EDS1 = 0xffffffff;
		flag = (*INTERRUPT_IRQ_PENDING2 >> 17) & (0xf);
		eds0 = *GPIO_EDS0;
		eds1 = *GPIO_EDS1;
		printf("gpio_1 0x%x\r\n", flag);
		printf("eds0: 0x%08x\r\n", eds0);
		printf("eds1: 0x%08x\r\n\n", eds1);

		count++;
	}

	// Basic IRQ pendingをチェック
	if((*INTERRUPT_IRQ_BASIC_PENDING & 0x01) != 0){
		// タイマー割り込み

		// デバッグ用
		// printf("Timer IRQ start\r\n");
		// printf("Timer Raw IRQ before: 0x%08x\r\n",*TIMER_RAWIRQ);
		// printf("irq_pending0 before : 0x%08x\r\n",*INTERRUPT_IRQ_BASIC_PENDING);

		// タイマ割り込み処理
		(*timer_IRQ_func)();

		// 割り込みフラグクリア
		*TIMER_IRQ_CLR = 0;
		printf("timer_irq\r\n");

		// デバッグ用
		// フラグがクリアされたかチェック
		// printf("irq_pending0 after: 0x%08x\r\n",*INTERRUPT_IRQ_BASIC_PENDING);
		// printf("Timer Raw IRQ after: 0x%08x\r\n",*TIMER_RAWIRQ);
		// printf("Timer IRQ end\r\n");
	}
	// TODO: その他の割り込みも調べる

	// IRQ割り込みを許可
	enable_IRQ();

	return;
}
