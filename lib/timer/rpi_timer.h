#ifndef RPILIB_TIMER_H
#define RPILIB_TIMER_H

// タイマーには二種類あり、
// システムタイマーと普通のタイマーがあります。
// 現在の時刻を知りたいときは主にシステムタイマー(syst)を用います。
// タイマー割り込みをしたいときは普通のタイマー(timer)を用います。


// システムタイマー
// init
void syst_init(void);

// 現在のタイマー(カウンタ)の値を取得する。
// これがタイマーの値を得るには最速だが、
// 後からタイマーの周波数を変更する可能性があるので、
// 速度がいらず、時間の単位が変わると困るものには
// 下の二つの関数(syst_mills, syst_micros)を
// 使用してください。
unsigned long long int syst_get_time(void);

// 単位をmsに直したもの
#define syst_mills() (syst_get_time()/1000)
// 単位をusに直したもの
#define syst_micros() syst_get_time()

// タイマー設定用マスク（ORマスクで利用してください）
#define	TIMER_DBG_STOP	(0x00000100)
#define TIMER_EN			(0x00000080)
#define TIMER_INT_EN		(0x00000020)
#define TIMER_BIT_23		(0x00000002)

// 割り込み用のタイマー
// 初期化。割り込み周期(us)を指定。
void timer_init(unsigned int period);
// 割り込み周期(us)を設定
void timer_set_period(unsigned int period);
// タイマーのカウントをスタート(割り込みは有効化しない)
void timer_start(void);
// タイマーのカウントをストップ
void timer_stop(void);
// タイマーの値を取得
unsigned int timer_read(void);
// タイマー割り込みの際に実行する関数を設定し、
// タイマーをスタート&割り込み有効化
void timer_attach_interrupt(void (*f)(void));
// タイマーの割り込み無効化
void timer_dettach_interrupt(void);

// タイマー回り込み処理用関数ポインタ
extern void (*timer_IRQ_func)(void);

#endif
