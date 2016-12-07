#include "rpi.h"

#include <stdio.h>

extern unsigned int getmode();

volatile unsigned int cnt;

int main(void) {
  rpi_init();

  while(1) {
    printf("Hello, World!\r\n");
  }
  return 0;
}
