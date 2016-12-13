#include "rpi.h"

#include <stdio.h>

extern void enable_IRQ();

int main(void) {
  rpi_init();

  // set chip-rate 10kHz
  // this must be larger than 4.7kHz
  pwm_set_frequency(10000);
  // set waveform 0000_0000_0000_0001_0111_1111_1111_1111
  pwm_set_data(0x0000ffff);
  // start output
  pwm_start();

  int state = 0;
  while(1) {
    printf("Hello, World!\r\n");
    for(int i = 0; i < 100000; i++) {
      asm("nop");
    }
  }
  return 0;
}
