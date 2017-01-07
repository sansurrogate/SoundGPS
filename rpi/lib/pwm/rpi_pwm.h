#ifndef RPI_PWM_H
#define RPI_PWM_H

#include <stdint.h>

void pwm_init();

void pwm_start();
void pwm_stop();

// set chip rate freq [Hz]
// this freq must be larger than 4.7k [Hz]
void pwm_set_frequency(unsigned int freq);
// tell the device data bit length
// this must be smaller than 32 [bit]
void pwm_set_len(unsigned int len);
// set data
void pwm_set_data(uint32_t *buf, unsigned int len);
void pwm_quick_reload();
void pwm_rescue();
void pwm_debug_info();

#endif /* end of include guard: RPI_PWM_H */
