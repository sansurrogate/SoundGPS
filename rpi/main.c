#include "rpi.h"

#include <stdio.h>

#define BEEP_DURATION 200000 // 0.2ms
int beep_flag = 0;
long int stop_time = 0;

void beep() {
  pwm_start();
  stop_time = syst_micros() + BEEP_DURATION;
  beep_flag = 1;
}

void toggle_pin() {
  static int state = 0;
  gpio_write(9, state);
  state ^= 1;
}

int main(void) {
  rpi_init();

  gpio_set_pin_mode(9, GPIO_OUTPUT);
  synchronize_attach_interrupt(beep);
  timer_attach_interrupt(toggle_pin);

  interrupt_enable_IRQ();

  while(1) {
    if(beep_flag) {
      if(syst_micros() > stop_time) {
        pwm_stop();
        beep_flag = 0;
      }
    }
  }
  return 0;
}
