#include "periphs/rpi_periphs.h"
#include "gpio/rpi_gpio.h"
#include <stdio.h>

// Default oscillator value is 19.2 [MHz]
#define OSC_FREQUENCY 19200000
#define PWM_CLK_PASSWORD 0x5a000000

#ifndef PWM_DEFAULT_FREQ
#define PWM_DEFAULT_FREQ 32000
#endif

#define CM_PWMCTL_BUSY (1 << 7)
#define CM_PWMCTL_ENAB (1 << 4)

#if 0
#define TO_DMAC_PERIADDR(x) (((x)&0x00ffffff) | 0x7e000000)
// #define TO_DMAC_MEMADDR(x) ((x) | 0x40000000)
#define TO_DMAC_MEMADDR(x) ((x) | 0xc0000000)
#else
#define TO_DMAC_PERIADDR(x) (((x)&0x00ffffff) | 0x7e000000)
#define TO_DMAC_MEMADDR(x) (x)
#endif

typedef struct {
  uint32_t TI;
  uint32_t SOURCE_AD;
  uint32_t DEST_AD;
  uint32_t TXFR_LEN;
  uint32_t STRIDE;
  uint32_t NEXTCONBK;
  uint32_t reserve1;
  uint32_t reserve2;
} ControlBlock_t;

static uint32_t dummy[] = {0x00000000};

static ControlBlock_t CB __attribute__ ((aligned(32)));

void pwm_set_frequency(unsigned int freq);
void pwm_set_len(unsigned int len);
void pwm_set_data(uint32_t *buf, unsigned int len);
void pwm_start();
void pwm_stop();
void pwm_quick_reload();
void pwm_rescue();
void pwm_debug_info();

void pwm_init() {
  // gpio_set_pin_mode(18, GPIO_ALT5);
  gpio_set_pin_mode(12, GPIO_INPUT_PULLDOWN);
  gpio_set_pin_mode(12, GPIO_ALT0);

  *DMA_INT_STATUS = 0;
  *DMA_ENABLE = 1;
  pwm_set_frequency(PWM_DEFAULT_FREQ);
  pwm_set_len(32);
  pwm_set_data(dummy, 0);
  pwm_stop();
}

// chip rate must be larger than 4.7kHz
void pwm_set_frequency(unsigned int freq) {
  // this devisor must fit 23:12 bits
  unsigned int osc_clock_divisor = (OSC_FREQUENCY / freq);

  unsigned int cache = *PWM_CTL;
  // stop pwm
  *PWM_CTL = 0;

  // refer to "BCM2835 Audio and PWM Clocks" by G.J. van Loo 6 Feb 2013
  // kill = 1; stop clock gen, clock src = 1; use oscillator clock
  *CM_PWMCTL = PWM_CLK_PASSWORD | (~CM_PWMCTL_ENAB & 0x1);
  while(*CM_PWMCTL & CM_PWMCTL_BUSY) {
    asm("nop");
  }

  // set frequency
  *CM_PWMDIV = PWM_CLK_PASSWORD | (osc_clock_divisor << 12);
  // start pwm clock, without mash filter
  *CM_PWMCTL = PWM_CLK_PASSWORD | (0x201 | CM_PWMCTL_ENAB);
  while(!(*CM_PWMCTL & CM_PWMCTL_BUSY)) {
    asm("nop");
  }

  *PWM_CTL = cache;
}

void pwm_start() {
  // disable pwm
  *PWM_CTL &= ~(1 << 0);
  // disable dreq
  *PWM_DMAC &= ~(1 << 31);
  // disable and reset dma
  *DMA0_CS = 0x80000000;
  // clear PWM FIFO
  *PWM_CTL |= (0x1 << 6);

  // ENAB=1, PANIC=1, DREQ=1
  *PWM_DMAC = (1 << 31) | (1 << 8) | (1 << 0);

  // use FIFO, serialiser mode, enable PWM_CTL
  *PWM_CTL = (1 << 5) | (1 << 1) | (1 << 0);

  // start dma
  // *DMA0_CS &= ~0x00000001;
  // *DMA0_CS |= 0x80000000;
  *DMA0_CONBLK_AD = TO_DMAC_MEMADDR((uint32_t)&CB);
  *DMA0_CS = 0x00000001;

  // clear pwm fifo & bus error
  if(*PWM_STA) {
    *PWM_STA = 0xffffffff;
  }
  // printf("pwm_start\r\n");
}

void pwm_stop() {
  // disable dma
  *DMA0_CS &= ~0x1;
  // *DMA0_CS = 0x80000000;

  // disable pwm
  *PWM_CTL = 0;
}

void pwm_set_len(unsigned int len) {
  *PWM_RNG1 = len;
}

void pwm_set_data(uint32_t *buf, unsigned int len) {
  // PERMAP=PWM, SRC_INC=1, DEST_DREQ=1, WAIT_RESP=1
  CB.TI = (0x5 << 16) | (0x1 << 8) | (0x1 << 6) | (0x1 << 3);
  CB.SOURCE_AD = TO_DMAC_MEMADDR((uint32_t)buf);
  CB.DEST_AD = TO_DMAC_PERIADDR((uint32_t)PWM_FIF1);
  CB.TXFR_LEN = 4 * len;
  CB.STRIDE = 0;
  CB.NEXTCONBK = TO_DMAC_MEMADDR((uint32_t)&CB);
  CB.reserve1 = 0;
  CB.reserve2 = 0;

  // *DMA0_CS &= ~0x1;
  // *DMA0_CS |= 0x80000000;

  // *PWM_DAT1 = data;
}

void pwm_rescue() {
  static int rescue_cnt = 0;
  volatile uint32_t dma_next_cblk = *DMA0_NEXTCONBLOCK;
  if(dma_next_cblk == 0) {
    *DMA0_CS &= ~0x1;
    *DMA0_CS |= 0x80000000;
    *DMA0_CONBLK_AD = TO_DMAC_MEMADDR((uint32_t)&CB);
    *DMA0_CS |= 0x00000001;
    rescue_cnt++;
  }
  printf("rescued %d times.\r\n", rescue_cnt);
}

void pwm_quick_reload() {
  // disable dma
  *DMA0_CS &= ~0x1;
  // clear PWM FIFO
  *PWM_CTL |= (0x1 << 6);
  // disable pwm
  *PWM_CTL &= ~(1 << 0);
  // disable dreq
  *PWM_DMAC &= ~(1 << 31);

  // abort DMA
  *DMA0_CS |= (0x1 << 30);

  // enable dreq
  *PWM_DMAC |= (1 << 31);
  // enable pwm
  *PWM_CTL |= (1 << 0);
  // enable dma
  *DMA0_CS |= 0x1;
}

void pwm_debug_info() {
  volatile uint32_t pwm_dmac = *PWM_DMAC;
  volatile uint32_t pwm_state = *PWM_STA;
  volatile uint32_t dma_cs = *DMA0_CS;
  volatile uint32_t dma_next_cblk = *DMA0_NEXTCONBLOCK;
  volatile uint32_t dma_debug = *DMA0_DEBUG;

  printf("CB address is 0x%08x\r\n", &CB);
  printf("pwm dmac is 0x%08x\r\n", pwm_dmac);
  printf("pwm status is 0x%08x\r\n", pwm_state);
  printf("dma cs is 0x%08x\r\n", dma_cs);
  printf("dma nextconblock is 0x%08x\r\n", dma_next_cblk);
  printf("dma debug is 0x%08x\r\n\n", dma_debug);
}
