#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            fprintf(stderr,"ERROR: CANNOT READ INPUTS OR DATA IS SHOTER THAN CODE\n");
            return 1;
        }
    //計算

    
    for(int j = 0; j < i - CODE_PERIOD; j++)
        {
            //fprintf(stderr,"%lf #DATA\n",d[j]);
            double res[2] = {0,0};
            for(int channel = 0; channel < CHANNEL_NUM;channel++)
                {
                    for(int k = 0; k < CODE_PERIOD; k++)
                        {
                            res[channel] += d[j + k] * (-1 * code[channel][k]);
                        }
                    
                }
            printf("%lf %lf\n",res[0],res[1]);
            
        }
    
    

    return 0;
}
