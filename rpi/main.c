#include "rpi.h"

#include <stdio.h>

#define BEEP_DURATION 100000 // 0.1s
volatile int beep_flag = 0;
volatile unsigned long long int stop_time = 0;

void beep() {
  unsigned long long int temp_time = syst_micros();
  pwm_start();
  // printf("b\r\n");
  unsigned long long int temp_stop_time = temp_time + BEEP_DURATION;
  stop_time = temp_stop_time;
  beep_flag = 1;
  // printf("c\r\n");
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
      unsigned long long int temp_time = syst_micros();
      if(temp_time > stop_time) {
        pwm_stop();
        beep_flag = 0;
        printf("a\r\n");
      }
    }
  }
  return 0;
}
