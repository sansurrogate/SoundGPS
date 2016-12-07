#include "rpi.h"

#include <stdio.h>

extern unsigned int getmode();

volatile unsigned int cnt;

void hello(void) {
  printf("%d\r\n", cnt);
  cnt = 0;
}

int main(void) {
  cnt = 0;
  rpi_init();

  pinMode(47, OUTPUT);
  int s = 1;

  Timer_attachInterrupt(hello);

  while(1) {
    // digitalWrite(47, s);
    // s ^= 1;
    // printf("hello\r\n");
    // for(int i = 0; i < 10000; i++) {
    //   asm("nop");
    // }
    cnt++;
  }
  return 0;
}
