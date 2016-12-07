#ifndef RPILIB_TIMER_H
#define RPILIB_TIMER_H

// init
void syst_init(void);

// systime
unsigned long long int syst_get_time(void);

#define syst_mills()		(syst_get_time()/1000)
#define syst_micros()	syst_get_time()

// タイマー設定用マスク（ORマスクで利用してください）
#define	TIMER_DBG_STOP	(0x00000100)
#define TIMER_EN			(0x00000080)
#define TIMER_INT_EN		(0x00000020)
#define TIMER_BIT_23		(0x00000002)


void timer_init(unsigned int period);
void timer_set_period(unsigned int period);
void timer_start(void);
void timer_stop(void);
unsigned int timer_read(void);
void timer_attach_interrupt(void (*f)(void));
void timer_dettach_interrupt(void);

// タイマー回り込み処理用関数ポインタ
extern void (*timer_IRQ_func)(void);

#endif
