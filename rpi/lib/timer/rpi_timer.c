#include "rpi_timer.h"
#include "periphs/rpi_periphs.h"
#include "type/rpi_type.h"
#include "sysconfig/rpi_sysconfig.h"
#include <stdio.h>

extern unsigned int getmode(void);
extern void enable_IRQ(void);

// Timer IRQ function
void (*timer_IRQ_func)(void);


void syst_init(void){
	*SYST_CHI = 0;
	*SYST_CLO = 0;
}

unsigned long long int syst_get_time(void){
	unsigned long long int t;
	unsigned int chi;
	unsigned int clo;

	// カウンタの値を取得
	chi = *SYST_CHI;
	clo = *SYST_CLO;

	// 桁上りチェック
	if(chi !=  *SYST_CHI){
		// 桁上りが起こっているならCHIとCLOを更新する
		chi = *SYST_CHI;
		clo = *SYST_CLO;
	}

	// 64bitに組み直す
	t = chi;
	t = t << 32;
	t += clo;

	return t;
}


void timer_enable_interrupt(){
	// タイマ割り込み許可
	*INTERRUPT_ENABLE_BASIC_IRQS |= 0x01;
	*TIMER_CONTROL |= TIMER_INT_EN;
}


void timer_disable_interrupt(){
	// タイマ割り込み不許可
	*INTERRUPT_ENABLE_BASIC_IRQS &= ~0x01;
	*TIMER_CONTROL &= ~TIMER_INT_EN;
}


int timer_set_clock(unsigned int clock){
	// 単位はHz
	if(clock > APB_CLOCK){
		// error
		return -1;
	}

	unsigned int prediv = APB_CLOCK / clock - 1;

	// printf("prediv = %d\n",prediv);
	// delay(10000);

	// pre-divider設定
	*TIMER_PREDIVIDER = prediv;

	return 0;
}


unsigned int timer_get_rawIRQ(void){
	return *TIMER_RAWIRQ;
}

unsigned int timer_get_maskedIRQ(void){
	return *TIMER_MASKEDIRQ;
}

void timer_clear_flag(void){
	*TIMER_IRQ_CLR = 0;
}

// Arduino風関数

void timer_init(unsigned int period){
	// timer clock を1MHzに設定
	timer_set_clock(1000000);

	// Timer 23bit
	*TIMER_CONTROL |= TIMER_BIT_23;

	// period(us)
	timer_stop();
	// Timer_start(period);
	timer_set_period(period);
}

void timer_set_period(unsigned int period){
	// period(us)
	*TIMER_RELOAD = period;
	*TIMER_LOAD = period;
}

void timer_start(void){
	// Timer Start
	*TIMER_CONTROL |= TIMER_EN;
}

void timer_stop(void){
	// Timer stop
	*TIMER_CONTROL &= ~TIMER_EN;
}

unsigned int timer_read(void){
	return *TIMER_VALUE;
}

void timer_attach_interrupt(void (*f)(void)){
	// printf("func will be attached.\r\n");
	// 割り込みフラグクリア
	*TIMER_IRQ_CLR = 1;
	// 関数ポインタセット
	timer_IRQ_func = f;

	// タイマー開始
	timer_start();

	// 割り込み有効
	timer_enable_interrupt();
	// printf("CPSR (in timer attaching) = 0x%08x\r\n",getmode());
	enable_IRQ();
	// printf("attaching finished\r\n");
}

void timer_dettach_interrupt(void){
	// disable interrupt
	timer_disable_interrupt();
}
