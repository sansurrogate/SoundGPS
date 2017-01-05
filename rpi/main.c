#include "rpi.h"

#include <stdio.h>

#define BEEP_DURATION 100000 // 0.1s
volatile int beep_flag = 0;
volatile unsigned long long int stop_time = 0;

#define N 32
static uint32_t data[N];

// void beep() {
//   unsigned long long int temp_time = syst_micros();
//   pwm_start();
//   // printf("b\r\n");
//   // unsigned long long int temp_stop_time = temp_time + BEEP_DURATION;
//   // stop_time = temp_stop_time;
//   stop_time = temp_time + BEEP_DURATION;
//   beep_flag = 1;
//   // printf("c\r\n");
// }

unsigned int cnt_syncro = 0;
unsigned int cnt_toggle = 0;

void syncro() {
  pwm_start();
  // printf("hoge\r\n");
  cnt_syncro++;
}

void toggle_pin() {
  static int state = 0;
  state ^= 1;
  cnt_toggle++;
  if(!state) {
    pwm_stop();
  }

  gpio_write(9, state);
  // printf("huga\r\n");
}

int main(void) {
  rpi_init();

  gpio_set_pin_mode(9, GPIO_OUTPUT);

  for(int i = 0; i < N; i++) {
    data[i] = 0x80000000;
  }
  pwm_set_data(data, N);

  timer_set_period(500000);

  synchronize_attach_interrupt(syncro);
  timer_attach_interrupt(toggle_pin);

  interrupt_enable_IRQ();

  // pwm_start();

  while(1) {
    // if(beep_flag) {
    //   unsigned long long int temp_time = syst_micros();
    //   if(temp_time > stop_time) {
    //     pwm_stop();
    //     beep_flag = 0;
    //     printf("a\r\n");
    //   }
    // }
    // for(int i = 0; i < 32; i++) {
    //   printf("dest[%02d]=0x%08x\r\n", i, dest_ary[i]);
    // }

    // pwm_start();
    asm("nop");
    printf("syncro: %d, toggle: %d\r\n", cnt_syncro, cnt_toggle);
    // printf("data address is 0x%08x\r\n", data);
    // pwm_debug_info();
    // pwm_rescue();
  }
  return 0;
}
