#include "periphs/rpi_periphs.h"
#include "gpio/rpi_gpio.h"

void (*synchronize_IRQ_func)(void);

// int pin_num = 10;
#define PIN_NUM 10

void synchronize_init() {
  gpio_set_pin_mode(PIN_NUM, GPIO_INPUT_PULLDOWN);
  gpio_set_pin_mode(PIN_NUM, GPIO_INPUT);
}

void synchronize_enable_interrupt() {
  // clear event register
  // *GPIO_EDS0 = (1 << PIN_NUM);
  *GPIO_EDS0 = 0xffffffff;
  *GPIO_EDS1 = 0xffffffff;

  // enable asynchronous rising event interruption
  // *GPIO_AREN0 = (1 << PIN_NUM);
  *GPIO_REN0 = (1 << PIN_NUM);

  // enable gpio interrupt
  *INTERRUPT_ENABLE_IRQS2 |= (1 << 20);
}

void synchronize_disable_interrupt() {
  *INTERRUPT_ENABLE_IRQS2 &= ~(1 << 20);
  // *GPIO_AREN0 &= ~(1 << PIN_NUM);
  *GPIO_REN0 &= ~(1 << PIN_NUM);
}

void synchronize_attach_interrupt(void (*f)(void)) {
  synchronize_IRQ_func = f;
  synchronize_enable_interrupt();
}

void synchronize_dettach_interrupt() {
  synchronize_disable_interrupt();
}
