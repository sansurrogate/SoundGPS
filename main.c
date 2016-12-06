#include "rpi.h"

#include <stdio.h>

extern unsigned int getmode();

void hello(void) {
  printf("hello, world.\r\n");
}

int main(void) {
  rpi_init();

  printf("init finished\r\n");
  pinMode(47, OUTPUT);
  int s = 1;

  // Timer_attachInterrupt(hello);

  while(1) {
    digitalWrite(47, s);
    s ^= 1;
    // printf("hello\r\n");
    for(int i = 0; i < 10000; i++) {
      asm("nop");
    }
  }
  return 0;
}
