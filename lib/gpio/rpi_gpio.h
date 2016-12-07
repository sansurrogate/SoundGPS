#ifndef RPILIB_GPIO_H
#define RPILIB_GPIO_H

#define GPIO_HIGH	1
#define GPIO_LOW		0

// GPIO mode
#define GPIO_INPUT	0x01
#define GPIO_OUTPUT	0x00
#define GPIO_ALT0	0x04
#define GPIO_ALT1	0x05
#define GPIO_ALT2	0x06
#define GPIO_ALT3	0x07
#define GPIO_ALT4	0x03
#define GPIO_ALT5	0x02
#define GPIO_INPUT_PULLUP	0x08
#define GPIO_INPUT_PULLDOWN	0x09

// init
void gpio_init(void);

// functions
void gpio_set_pullup_pulldown(int pin,int mode);
void gpio_set_pin_mode(int pin,int mode);
void gpio_write(int pin, int value);
int gpio_read(int pin);
#endif
