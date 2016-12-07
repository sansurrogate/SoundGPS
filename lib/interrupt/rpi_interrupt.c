#include "timer/rpi_timer.h"
#include "periphs/rpi_periphs.h"
#include <stdio.h>
#include <stdint.h>

extern void enable_IRQ(void);
extern void disable_IRQ(void);
extern unsigned int getmode(void);

// void set_vector_table(void){
// 	extern void *_initial_vector_start;
// 	extern void *_initial_vector_end;
// 	// volatileをつけないと最適化に消される（涙目）
// 	volatile unsigned int *vec = 0;
// 	volatile unsigned int *p;
// 	volatile unsigned int *s = (unsigned int *)&_initial_vector_start;
// 	volatile unsigned int *e = (unsigned int *)&_initial_vector_end;
//
// 	printf("Vector table check\r\n");
// 	printf("Addr : Hex\r\n");
// 	printf("s: 0x%08x\r\n", s);
// 	printf("e: 0x%08x\r\n", e);
// 	for (p = s; p < e; p++) {
// 		printf("vector_table\r\n");
// 		// *vec = *p;
// 		printf("0x%02x : 0x%08x\r\n",vec,*vec);
// 		vec++;
// 	}
// }


void disable_all_IRQ(void){
	// すべての割り込み不許可
	*INTERRUPT_DISABLE_BASIC_IRQS = 0xffffffff;
	*INTERRUPT_DISABLE_IRQS1 = 0xffffffff;
	*INTERRUPT_DISABLE_IRQS2 = 0xffffffff;
	*INTERRUPT_FIQ_CTRL = 0;

	// IRQ不許可
	disable_IRQ();
}


// IRQ割り込みハンドラ
void IRQ_handler(void){
	// IRQ割り込みを停止
	disable_IRQ();
	// printf("0x%04x\r\n",getmode());

	// printf("get IRQ in IRQ handler\r\n");
	// printf("CPSR (in IRQ_handler) = 0x%08x\r\n",getmode());

	// Basic IRQ pendingをチェック
	if((*INTERRUPT_IRQ_BASIC_PENDING & 0x01) != 0){
		// タイマー割り込み

		// デバッグ用
		// printf("Timer IRQ start\r\n");
		// printf("Timer Raw IRQ before: 0x%08x\r\n",*TIMER_RAWIRQ);
		// printf("irq_pending0 before : 0x%08x\r\n",*INTERRUPT_IRQ_BASIC_PENDING);

		// タイマ割り込み処理
		(*timerIRQ_func)();

		// 割り込みフラグクリア
		*TIMER_IRQ_CLR = 0;

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
