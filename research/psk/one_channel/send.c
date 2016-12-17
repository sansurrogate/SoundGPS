#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define SAMPLING_FREQ 48000
#define CAREER_FREQ 6000
#define CHIP_RATE 375
#define CODE_PERIOD 31
#define CHANNEL_NUM 2
#define N (SAMPLING_FREQ * CODE_PERIOD / CHIP_RATE)

int main(int ARGC, char *ARGV[]) {
  int phy_offset[CHANNEL_NUM] = {0, 0};
  double amp[CHANNEL_NUM] = {0.2, 0.2};

  // 1チップあたりの波形
  double wave[2][SAMPLING_FREQ / CHIP_RATE];
  for(int i = 0; i < SAMPLING_FREQ / CHIP_RATE; i++) {
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
  for(int i = 0; i < CODE_PERIOD; i++) { // iはこのループが何番目のチップか
    for(int j = 0; j < SAMPLING_FREQ / CHIP_RATE; j++) { // jは
      for(int k = 0; k < CHANNEL_NUM; k++) {
        int symbol = code[k][i] ^ 1;
        data[k][i * SAMPLING_FREQ / CHIP_RATE + j] = wave[symbol][j];
      }
    }
  }

  // 実際に再生する波形
  int16_t buf[N];
  for(int i = 0; i < N; i++) {
    buf[i] = 0;
    for(int j = 0; j < CHANNEL_NUM; j++) {
      int idx = (N + i - phy_offset[j]) % N;
      buf[i] += (int16_t)(30000 * amp[j] * data[j][idx]);
    }
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
      n += fwrite(buf + n, sizeof(short), N - n, play);
      // n += fwrite(buf + n, sizeof(short), N - n, stdout);

      if(n >= N) {
        break;
      }
    }
  }

  pclose(play);

  return 0;
}
