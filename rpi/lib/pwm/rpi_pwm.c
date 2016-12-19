#include "periphs/rpi_periphs.h"
#include "gpio/rpi_gpio.h"

// Default oscillator value is 19.2 [MHz]
#define OSC_FREQUENCY 19200000
#define PWM_CLK_PASSWORD 0x5a000000

#ifndef PWM_DEFAULT_FREQ
#define PWM_DEFAULT_FREQ 5000
#endif

void pwm_set_frequency(unsigned int freq);
void pwm_set_len(unsigned int len);
void pwm_set_data(unsigned int data);
void pwm_start();
void pwm_stop();

#define CM_PWMCTL_BUSY (1 << 7)
#define CM_PWMCTL_ENAB (1 << 4)

void pwm_init() {
  // gpio_set_pin_mode(18, GPIO_ALT5);
  gpio_set_pin_mode(12, GPIO_INPUT_PULLDOWN);
  gpio_set_pin_mode(12, GPIO_ALT0);

  pwm_set_frequency(PWM_DEFAULT_FREQ);
  pwm_set_len(32);
  pwm_set_data(0xaaaaaaaa);
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
  // without no algorythm, enable PWM_CTL
  *PWM_CTL = 0x00000003;
}

void pwm_stop() {
  *PWM_CTL = 0;
}

void pwm_set_len(unsigned int len) {
  *PWM_RNG1 = len;
}

void pwm_set_data(unsigned int data) {
  *PWM_DAT1 = data;
}
