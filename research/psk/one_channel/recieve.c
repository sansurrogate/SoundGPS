#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
  char *text_file_name, *raw_file_name;
  int text_file_name_len = strlen("data/") + strlen(ARGV[1]) + strlen(".txt") + 1;
  int raw_file_name_len = strlen("data/") + strlen(ARGV[1]) + strlen(".raw") + 1;
  text_file_name = (char *)malloc(sizeof(char) * text_file_name_len);
  raw_file_name = (char *)malloc(sizeof(char) * raw_file_name_len);
  strcpy(text_file_name, "data/");
  strcpy(raw_file_name, "data/");
  strcat(text_file_name, ARGV[1]);
  strcat(raw_file_name, ARGV[1]);
  strcat(text_file_name, ".txt");
  strcat(raw_file_name, ".raw");

  FILE *text, *raw;
  if ((text = fopen(text_file_name, "w")) == NULL) {
    perror("fopen");
    exit(1);
  }
  if ((raw = fopen(raw_file_name, "w")) == NULL) {
    perror("fopen");
    exit(1);
  }

  for(int i = 0; i < N; i++) {
    fprintf(text, "%d %d\n", i, buf[i]);
    fwrite(&buf[i], sizeof(int16_t), 1, raw);
  }

  fclose(text);
  fclose(raw);
  pclose(rec);

  return 0;
}
