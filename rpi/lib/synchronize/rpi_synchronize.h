#ifndef RPI_SYNCHRONIZE_H
#define RPI_SYNCHRONIZE_H

void synchronize_init();
void synchronize_attach_interrupt(void (*f)(void));

extern void (*synchronize_IRQ_func)(void);

#endif /* end of include guard: RPI_SYNCHRONIZE_H */
