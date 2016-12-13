#include "rpi.h"

#include <stdio.h>

extern void enable_IRQ();

int main(void) {
  rpi_init();

  int pin_num = 10;

  gpio_set_pin_mode(9, GPIO_OUTPUT);
  gpio_set_pin_mode(47, GPIO_OUTPUT);
  gpio_set_pin_mode(pin_num, GPIO_INPUT_PULLDOWN);
  gpio_set_pin_mode(pin_num, GPIO_INPUT);

  // clear event register
  *GPIO_EDS0 = (1 << pin_num);
  *GPIO_AREN0 = (1 << pin_num);
  *INTERRUPT_ENABLE_IRQS2 |= (1 << 20);
  enable_IRQ();


  // set chip-rate 10kHz
  // this must be larger than 4.7kHz
  pwm_set_frequency(10000);
  // set waveform 0000_0000_0000_0001_0111_1111_1111_1111
  pwm_set_data(0x0000ffff);
  // start output
  pwm_start();

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
