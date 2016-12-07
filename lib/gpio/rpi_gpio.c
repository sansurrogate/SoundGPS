#include "rpi_gpio.h"
#include "periphs/rpi_periphs.h"
#include "type/rpi_type.h"

void gpio_init(void){
	int i;
	// pullup all
	*GPIO_PUD 	= 0x02;
	// wait 150 cycle
	for(i=0;i<150;i++){
		// nop
		asm("nop");
	}
	*GPIO_PUDCLK0 = 0xffffffff;
	*GPIO_PUDCLK1 = 0xffffffff;
	// wait 150 cycle
	for(i=0;i<150;i++){
		// nop
		asm("nop");
	}
	*GPIO_PUDCLK0 = 0;
	*GPIO_PUDCLK1 = 0;

	*GPIO_FSEL0 = 0;
	*GPIO_FSEL1 = 0;
	*GPIO_FSEL2 = 0;
	*GPIO_FSEL3 = 0;
	*GPIO_FSEL4 = 0;
	*GPIO_FSEL5 = 0;
}

void gpio_set_pullup_pulldown(int pin,int mode){
	int i;
	vu32_t *res;

	if(pin <= 32){
		res = GPIO_PUDCLK0;
	}
	else{
		res = GPIO_PUDCLK1;
	}
	// set PULLUP
	if(mode == GPIO_INPUT_PULLUP){
		*GPIO_PUD 	= 0x02;
	}else if (mode == GPIO_INPUT_PULLDOWN){
		*GPIO_PUD 	= 0x01;
	}else{
		return;
	}
	// wait 150 cycle
	for(i=0;i<150;i++){
		// nop
		asm("nop");
	}
	*res = (0x01 << (pin % 32));
	// wait 150 cycle
	for(i=0;i<150;i++){
		// nop
		asm("nop");
	}
	*res = 0;
	return;
}


void gpio_set_pin_mode(int pin,int mode){
	vu32_t *res;

	// GPFSEL select
	if(0 <= pin && pin <= 9){
		res = GPIO_FSEL0;
	}else if (pin <= 19)
	{
		res = GPIO_FSEL1;
	}else if (pin <= 29)
	{
		res = GPIO_FSEL2;
	}else if (pin <= 39)
	{
		res = GPIO_FSEL3;
	}else if (pin <= 49)
	{
		res = GPIO_FSEL4;
	}else if (pin <= 53)
	{
		res = GPIO_FSEL5;
	}else{
		// pin missmuch
		return;
	}

	// mode set
	switch(mode){
		case GPIO_INPUT:
			*res &= GPIO_FSEL_MASK_IN(pin);
			break;
		case GPIO_INPUT_PULLUP:
			gpio_set_pullup_pulldown(pin,GPIO_INPUT_PULLUP);
			*res &= GPIO_FSEL_MASK_IN(pin);
			break;
		case GPIO_INPUT_PULLDOWN:
			gpio_set_pullup_pulldown(pin,GPIO_INPUT_PULLDOWN);
			*res &= GPIO_FSEL_MASK_IN(pin);
			break;
		case GPIO_OUTPUT:
			*res |= GPIO_FSEL_MASK_OUT(pin);
			break;
		case GPIO_ALT0:
			*res |= GPIO_FSEL_MASK_ALT0(pin);
			break;
		case GPIO_ALT1:
			*res |= GPIO_FSEL_MASK_ALT1(pin);
			break;
		case GPIO_ALT2:
			*res |= GPIO_FSEL_MASK_ALT2(pin);
			break;
		case GPIO_ALT3:
			*res |= GPIO_FSEL_MASK_ALT3(pin);
			break;
		case GPIO_ALT4:
			*res |= GPIO_FSEL_MASK_ALT4(pin);
			break;
		case GPIO_ALT5:
			*res |= GPIO_FSEL_MASK_ALT5(pin);
			break;
		default:
			// error!
			;
	}
	return;
}


void gpio_write(int pin, int value){
	vu32_t *res;

	if(value == GPIO_HIGH){
		// GPSET select
		if(0 <= pin && pin <= 32){
			res = GPIO_SET0;
		}else if (pin <= 53)
		{
			res = GPIO_SET1;
		}else{
			// pin missmuch
			return;
		}
	}
	else if(value == GPIO_LOW){
		// GPCLR select
		if(0 <= pin && pin <= 32){
			res = GPIO_CLR0;
		}else if (pin <= 53)
		{
			res = GPIO_CLR1;
		}else{
			// pin missmuch
			return;
		}
	}
	else{
		return;
	}
	// output
	*res |= (0x01 << (pin % 32));
	return;
}


int gpio_read(int pin){
	unsigned int val;
	unsigned int mask;

	// GPCLR select
	if(0 <= pin && pin <= 32){
		val = *GPIO_LEV0;
	}else if (pin <= 53)
	{
		val = *GPIO_LEV1;
	}else{
		// pin missmuch
		return -1;
	}

	// input
	mask = 0x01 << (pin % 32);
	if((val & mask) != 0){
		return GPIO_HIGH;
	}
	return GPIO_LOW;
}
