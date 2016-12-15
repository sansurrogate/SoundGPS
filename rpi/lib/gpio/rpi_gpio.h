#ifndef RPILIB_GPIO_H
#define RPILIB_GPIO_H

#define GPIO_HIGH 1
#define GPIO_LOW 0

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
// GPIOを入力用に設定する際は、以下のように
// pullup/downを設定してから、inputに設定する。
// gpio_set_pin_mode(47, GPIO_INPUT_PULLUP);
// gpio_set_pin_mode(47, GPIO_INPUT);

// GPIOを出力用に設定する際は、そのまま
// gpio_set_pin_mode(47, GPIO_OUTPUT);
// と、いきなりoutputに設定して良い。

void gpio_set_pullup_pulldown(int pin, int mode);
void gpio_set_pin_mode(int pin, int mode);
// 出力用のGPIOピンの出力電圧を設定
void gpio_write(int pin, int value);
// 入力用のGPIOピンの入力電圧を読み取り
int gpio_read(int pin);

#endif
