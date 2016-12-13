#include "rpi.h"

#include <stdio.h>

void hello() {
  printf("Hello, gpio irq.\r\n");
}

int main(void) {
  rpi_init();

  synchronize_attach_interrupt(hello);

  gpio_set_pin_mode(9, GPIO_OUTPUT);

  interrupt_enable_IRQ();

  int state = 0;
  while(1) {
    gpio_write(9, state);
    state ^= 1;
    for(int i = 0; i < 100000; i++) {
      asm("nop");
    }
  }
  return 0;
}
