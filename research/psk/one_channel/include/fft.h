#ifndef FFT_H
#define FFT_H

#include <complex.h>
#include <stdio.h>

typedef char sample_t;

void sample_to_complex(sample_t *s, complex double *X, long n);
void  complex_to_sample(complex double *X, sample_t *s, long n);

void fft(complex double *x, complex double *y, long n);
void ifft(complex double *y, complex double *x, long n);

void print_complex(FILE *wp, complex double *Y, long n);
void cat_freq(complex double *Y, long n, double f);

void fourier_transform(sample_t *buf, long n, double f, complex double *Y);

complex double get_value_at_specific_freq(complex double *Y, long n, double f, double sample_f);

#endif /* end of include guard: FFT_H */
