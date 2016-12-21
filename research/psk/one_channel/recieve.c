#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define M_PI 3.1415926535
#define SAMPLING_FREQ 48000
#define CAREER_FREQ 8000
#define CHIP_RATE 160
#define CODE_PERIOD 31
#define CHANNEL_NUM 2
// #define N (SAMPLING_FREQ * CODE_PERIOD / CHIP_RATE)
#define N 144000

int main(int ARGC, char *ARGV[]) {
  // recプログラムの起動
  FILE *rec;
  char cmd[] = "rec -t raw -b 16 -c 1 -e s -r 48000 -";
  if((rec = popen(cmd, "r")) == NULL) {
    perror("rec");
  }

  int n = 0;
  int16_t buf[N];

  // 3秒間ほど録音
  while(1) {
    n += fread(buf + n, sizeof(int16_t), N - n, rec);
    if(n >= N) {
      break;
    }
  }

  // ファイルに書き出し
  FILE *fp;
  if ((fp = fopen(ARGV[1], "w")) == NULL) {
    perror("fopen");
    exit(1);
  }

  for(int i = 0; i < N; i++) {
    fprintf(fp, "%d %d\n", i, buf[i]);
  }

  fclose(fp);
  pclose(rec);

  return 0;
}
