#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define CHANNEL_NUM 2
#define CODE_PERIOD 31


int DATA_LIMIT = 10000;

char code[CHANNEL_NUM][CODE_PERIOD] = {{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1,
                       1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0},
                      {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0,
                       0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1}};


int main()
{
    
    double *d = malloc(sizeof(double) * DATA_LIMIT);
    memset(d,0,sizeof(double) * DATA_LIMIT);
    
    int i = 0;
    while(0 < scanf("%*d %*f %*f %*f %*f %*f %lf %*d\n",d + i))
        {
            i++;
            if(i > DATA_LIMIT)
                {
                    DATA_LIMIT += 10000;
                    d = realloc(d,sizeof(double) * DATA_LIMIT);
                }
            
        }
    
    if(i < CODE_PERIOD)
        {
            fprintf(stderr,"ERROR: CANNOT READ INPUTS, OR DATA IS SHOTER THAN CODE\n");
            return 1;
        }
    //計算

    for(int j = 0; j < i / CODE_PERIOD; j++)
        {
            double res[CHANNEL_NUM][CODE_PERIOD] = {};
            double max = 0;
            int n_max[2] = {};
            //まずは，どちらの方が強度が強いかを測定する
            for(int channel = 0; channel < CHANNEL_NUM;channel++)
                {
                    for(int l = 0; l < CODE_PERIOD; l++) 
                        {
                            for(int k = 0; k < CODE_PERIOD; k++)
                                {
                                    res[channel][l] += d[j * CODE_PERIOD + k] * ( -2 * code[channel][(k - l + CODE_PERIOD) % CODE_PERIOD] + 1 );
                                }
                            
                            if( fabs(res[channel][l]) > max) {
                                max = fabs(res[channel][l]);
                                n_max[0] = channel;
                                n_max[1] = l;
                            }
                            
                        }
                }

            //強度が強い方に対して，データを引いて強度が弱い方の値を出す
            for(int channel = 0; channel < CHANNEL_NUM;channel++) 
                {
                    if(channel == n_max[0]) continue;
                    
                    for(int l = 0; l < CODE_PERIOD; l++) 
                        {
                            res[channel][l] = 0;
                            for(int k = 0; k < CODE_PERIOD; k++)
                                {
                                    res[channel][l] += (d[j * CODE_PERIOD + k] -  max / (double) CODE_PERIOD *(-2 * code[n_max[0]][(k - n_max[1] + CODE_PERIOD) % CODE_PERIOD] + 1)) * ( -2 * code[channel][(k - l + CODE_PERIOD) % CODE_PERIOD] + 1 );
                                }
                            printf("%f %f\n",res[0][l], res[1][l]);
                        }
                }
        }
    
    

    return 0;
}
