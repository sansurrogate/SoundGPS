#ifndef RPILIB_SERIAL_H
#define RPILIB_SERIAL_H

// この関数で設定した時間が過ぎたら、getcやputcはタイムアウトする
void serial_set_timeout(int read_ms,int write_ms);

// uart用のgetc, putc, puts, gets
int uart0_getc(void);
int uart0_putc(int c);
int uart0_puts(char *s);
char *uart0_gets(char *s,int len);

// uartによる通信を有効化
int serial_begin(unsigned int baudrate);
// uartによる通信を無効化
void serial_end(void);
// uartによる通信が現在有効かどうか
int serial_available(void);

// 指定したバイト数だけ書き込み
int serial_write(char *buf,int len);

// readは今の実装だと遅いので、使わないでください
// 一応、後のことを考えて形だけ定義してあります。
#define serial_read		uart0_getc

#endif
