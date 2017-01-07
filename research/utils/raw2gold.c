/* 
 * fft.c
 * 使い方
 *   ./fft n
 * 
 * 以下を繰り返す:
 *   標準入力から, 16 bit integerをn個読む
 *   FFTする
 *   逆FFTする
 *   標準出力へ出す
 *
 * したがって「ほぼ何もしない」フィルタになる
 * 
 */
#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

typedef int16_t sample_t;

void die(char * s) {
  perror(s); 
  exit(1);
}

/* fd から 必ず n バイト読み, bufへ書く.
   n バイト未満でEOFに達したら, 残りは0で埋める.
   fd から読み出されたバイト数を返す */
ssize_t read_n(int fd, ssize_t n, void * buf) {
  ssize_t re = 0;
  while (re < n) {
    ssize_t r = read(fd, buf + re, n - re);
    if (r == -1) die("read");
    if (r == 0) break;
    re += r;
  }
  memset(buf + re, 0, n - re);
  return re;
}

/* fdへ, bufからnバイト書く */
ssize_t write_n(int fd, ssize_t n, void * buf) {
  ssize_t wr = 0;
  while (wr < n) {
    ssize_t w = write(fd, buf + wr, n - wr);
    if (w == -1) die("write");
    wr += w;
  }
  return wr;
}

/* 標本(整数)を複素数へ変換 */
void sample_to_complex(sample_t * s, 
		       complex double * X, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) X[i] = s[i];
}

/* 複素数を標本(整数)へ変換. 虚数部分は無視 */
void complex_to_sample(complex double * X, 
		       sample_t * s, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) {
    s[i] = creal(X[i]);
  }
}

/* 高速(逆)フーリエ変換;
   w は1のn乗根.
   フーリエ変換の場合   偏角 -2 pi / n
   逆フーリエ変換の場合 偏角  2 pi / n
   xが入力でyが出力.
   xも破壊される
 */
void fft_r(complex double * x, 
	   complex double * y, 
	   long n, 
	   complex double w) {
  if (n == 1) { y[0] = x[0]; }
  else {
    complex double W = 1.0; 
    long i;
    for (i = 0; i < n/2; i++) {
      y[i]     =     (x[i] + x[i+n/2]); /* 偶数行 */
      y[i+n/2] = W * (x[i] - x[i+n/2]); /* 奇数行 */
      W *= w;
    }
    fft_r(y,     x,     n/2, w * w);
    fft_r(y+n/2, x+n/2, n/2, w * w);
    for (i = 0; i < n/2; i++) {
      y[2*i]   = x[i];
      y[2*i+1] = x[i+n/2];
    }
  }
}

void fft(complex double * x, 
	 complex double * y, 
	 long n) {
  long i;
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) - 1.0j * sin(arg);
  fft_r(x, y, n, w);
  for (i = 0; i < n; i++) y[i] /= n;
}

void ifft(complex double * y, 
	  complex double * x, 
	  long n) {
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) + 1.0j * sin(arg);
  fft_r(y, x, n, w);
}

int pow2check(long N) {
  long n = N;
  while (n > 1) {
    if (n % 2) return 0;
    n = n / 2;
  }
  return 1;
}

void print_complex(FILE * wp, 
		   complex double * Y, long n) {
  long i;
  for (i = 0; i < n; i++) {
    fprintf(wp, "%ld %f %f %f %f\n", 
	    i, 
	    creal(Y[i]), cimag(Y[i]),
	    cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}

void cat_freq( 
              complex double * Y, long n, double f) {
  long i;
  for (i = 0; i < n; i++) {
    printf( "%ld %f %f %f %f %f\n", 
            i,
            f*(double)i/(double)n,
	    creal(Y[i]), cimag(Y[i]),
	    cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}

void cat_one_freq( 
                  complex double * Y, long n, double f,double target) {
  long i;
  for (i = 0; i < n; i++) {
      if(fabs(target - f*(double)i / (double)n) < f / (double)n / 2) 
          {
              printf( "%ld %f %f %f %f %f\n", 
                      i,
                      f*(double)i/(double)n,
                      creal(Y[i]), cimag(Y[i]),
                      cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
          }
  }
  
}


complex double * W;
complex double * Z;

#define WINDOW_SIZE 4
#define GOLD_SIZE 31

void cat_init()
{
    W = malloc(sizeof(complex double) * WINDOW_SIZE);
    Z = malloc(sizeof(complex double) * GOLD_SIZE);
}

complex double  get_median()
{
    for(int i = 0; i < 3; i++)
        {
            if(atan2(cimag(W[i]), creal(W[i])) < atan2(cimag(W[i + 1]), creal(W[i + 1])))
                {
                    complex double swap = W[i];
                    W[i] = W[i + 1];
                    W[i + 1] = swap;
                    i = 0;
                }
            
        }
    return (W[1] + W[2]) / 2;
}

double get_offset()
{

    double offs = 0;
    for(int i = 0; i < GOLD_SIZE; i++)
        {
            offs += atan2(cimag(Z[i]), creal(Z[i]));
        }
    
    return offs / (double) GOLD_SIZE;
}


void cat_one_gold( 
                  complex double * Y, long n, double f,double target) {
  long i;
  static int w_sum = 0;
  
  static int z_sum = 0;
  
  
  for (i = 0; i < n; i++) {
      if(fabs(target - f*(double)i / (double)n) < f / (double)n / 2) 
          {
              W[w_sum] = Y[i];
              w_sum++;
              if(w_sum == WINDOW_SIZE) 
                  {
                      Z[z_sum] = get_median();
                      w_sum = 0;
                      z_sum++;
                  }

              if(z_sum == GOLD_SIZE)
                  {
                      double offs = get_offset();
                      for(int j = 0; j < GOLD_SIZE; j++)
                          {
                              printf( "%d %f %f %f %f %f %f %d\n", 
                                      j,
                                      f*(double)j/(double)n,
                                      creal(Z[j]), cimag(Z[j]),
                                      cabs(Z[j]),
                                      atan2(cimag(Z[j]), creal(Z[j])),
                                      cabs(Z[j]) * sin(atan2(cimag(Z[j]), creal(Z[j])) - offs),
                                      cabs(Z[j]) * sin(atan2(cimag(Z[j]), creal(Z[j])) - offs) < 0 ? 0 : 1
                                      );
                          }
                      z_sum = 0;
                  }
              
          }
  }
  
}



int main(int argc, char ** argv) {
  if(argc < 4) 
      {
          fprintf(stderr,"Usage: raw2freq.exe [sample sets n] [sampling freq Hz] [target freq Hz]");
          exit(1);
          
      }
  long n = atol(argv[1]);
  double f = atof(argv[2]);
  double target = atof(argv[3]);
  
  
  if (!pow2check(n)) {
    fprintf(stderr, "error : n (%ld) not a power of two\n", n);
    exit(1);
  }
  cat_init();

  
  FILE * wp = fopen("fft.dat", "wb");
  if (wp == NULL) die("fopen");
  sample_t * buf = calloc(sizeof(sample_t), n);
  complex double * X = calloc(sizeof(complex double), n);
  complex double * Y = calloc(sizeof(complex double), n);
  while (1) {
    /* 標準入力からn個標本を読む */
    ssize_t m = read_n(0, n * sizeof(sample_t), buf);
    if (m == 0) break;
    /* 複素数の配列に変換 */
    sample_to_complex(buf, X, n);
    /* FFT -> Y */
    fft(X, Y, n);

    //標準出力にデータを出力
    cat_one_gold(Y, n, f, target);

  }
  fclose(wp);
  return 0;
}