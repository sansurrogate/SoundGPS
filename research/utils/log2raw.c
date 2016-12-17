#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    char po[100000] = "";
    while(gets(po)) {
        strtok(po," ");
        char* pi = strtok(NULL," ");
        int16_t out = atoi(pi);
        
        fwrite(&out,sizeof(int16_t),1,stdout);
        memset(po,0,100000);
        
    }
    return 1;
    
}
