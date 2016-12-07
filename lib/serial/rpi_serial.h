#ifndef RPILIB_SERIAL_H
#define RPILIB_SERIAL_H

void serial_set_timeout(int read_ms,int write_ms);

int uart0_getc(void);
int uart0_putc(int c);
int uart0_puts(char *s);
char *uart0_gets(char *s,int len);

int serial_begin(unsigned int baudrate);
void serial_end(void);
int serial_available(void);

int serial_write(char *buf,int len);
#define serial_read		uart0_getc

#endif
