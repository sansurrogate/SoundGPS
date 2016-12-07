#ifndef RPI_INTERRUPT_H
#define RPI_INTERRUPT_H

// 関数定義
// void set_vector_table(void);
void interrupt_disable_all_IRQ(void);
void interrupt_IRQ_handler(void);

#endif /* end of include guard: RPI_INTERRUPT_H */
