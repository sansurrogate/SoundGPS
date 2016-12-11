#ifndef RPI_INTERRUPT_H
#define RPI_INTERRUPT_H

// 関数定義

// RPI2より前のボードは例外ベクタの位置が異なる
#ifndef RPI2
void interrupt_init_exception_vector(void);
#endif

// 全ての割り込みを無効にする
void interrupt_disable_all_IRQ(void);

// 割り込みハンドラ
// (IRQ割り込みbitが立てられたら、この関数が呼び出される)
void interrupt_IRQ_handler(void);

#endif /* end of include guard: RPI_INTERRUPT_H */
