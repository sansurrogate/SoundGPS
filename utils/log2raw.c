#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char po[100000] = "";
    while(gets(po)) {
        char* pa = strtok(po," ");
        char* pi = strtok(NULL," ");
        char out[2] = {atoi(pa),atoi(pi)};
        
        putchar(out[1]);
        memset(po,0,100000);
        
    }
    return 1;
    
}
