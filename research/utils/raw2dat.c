#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){
  //仕様として、ソース名だけもらったら48000のrawだと解釈して描画、２つ引数があったら2つ目を
  //sampling freq.として解釈しよう

  if(argc < 2) {
    fprintf(stderr,"Usage: ./raw2dat [rawfile] [freq(default 48000Hz)]\n");
    return 0;
  }

  FILE* fp = fopen(argv[1], "rb");
  double line = 0;
  double freq = (argc > 2) ? atoi(argv[2]) : 48000;
  int c;
  while((c = fgetc(fp)) != -1){
    printf("%f %d\n",line / freq,c);
    line++;
  }

  return 0;
}
