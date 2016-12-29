#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#define SAMPLING_FREQ 48000
#define CAREER_FREQ 6000
#define CHIP_RATE 375
#define CODE_PERIOD 31
#define CHANNEL_NUM 2

// 1チップあたりのサンプル数: sampling_rate / chip_rate
// ゴールド系列1周期分のサンプル数: 1チップあたりのサンプル数 * コード長
#define N (SAMPLING_FREQ * CODE_PERIOD / CHIP_RATE)

int main(int ARGC, char *ARGV[]) {
  int i, j, k;

  double amp[CHANNEL_NUM] = {0.5, 0.5};

  // 1チップあたりの波形
  double wave[2][SAMPLING_FREQ / CHIP_RATE];
  for(i = 0; i < SAMPLING_FREQ / CHIP_RATE; i++) {
    wave[0][i] = sin(2 * M_PI * CAREER_FREQ * i / SAMPLING_FREQ);
    wave[1][i] = sin(2 * M_PI * CAREER_FREQ * i / SAMPLING_FREQ + M_PI / 2);
  }

  // Gold系列
  char code[CHANNEL_NUM][CODE_PERIOD] = {{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1,
                       1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0},
                      {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0,
                       0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1}};

  // 各チャネルの波形
  double data[CHANNEL_NUM][N];
  for(i = 0; i < CODE_PERIOD; i++) { // iはこのループが何番目のチップか
    for(j = 0; j < SAMPLING_FREQ / CHIP_RATE; j++) { // jはi番目のチップ内で何番目のサンプルか
      for(k = 0; k < CHANNEL_NUM; k++) { // kはチャネル番号
        // 0の時位相を0, 1の時位相をpi/4だけずらす
        int symbol = code[k][i] ^ 1;
        data[k][i * SAMPLING_FREQ / CHIP_RATE + j] = wave[symbol][j];
      }
    }
  }

  // 実際に再生する波形
  int16_t buf[N];
  for(i = 0; i < N; i++) {
    buf[i] = 0;
    int ch = atoi(ARGV[1]);
    buf[i] = (int16_t)(INT16_MAX * amp[ch] * data[ch][i]);
  }


  // playプログラムの起動
  FILE *play;
  char cmd[] = "play -t raw -b 16 -c 1 -e s -r 48000 -";
  if((play = popen(cmd, "w")) == NULL) {
    perror("play");
  }

  int n;
  // 再生
  while(1) {
    n = 0;
    while(1) {
      n += fwrite(buf + n, sizeof(int16_t), N - n, play);

      if(n >= N) {
        break;
      }
    }
  }

  pclose(play);

  return 0;
}
