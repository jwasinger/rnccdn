#include <stdio.h>
#include <stdlib.h>
#include "mt64.h"

#define SPACE  600000
#define DATA_LENGTH   SPACE / sizeof(uint16_t)

uint16_t mt_rand16() {
    //uint16_t temp = genrand64_int64() & 0xffff;
    return genrand64_int64() & 0xffff;
}

int main(){
    uint16_t *input;
    
    int i;
    
    if ((input = malloc(DATA_LENGTH * sizeof *input)) == NULL) { // 16bit
        perror("malloc");
        exit(1);
    }
    
    for (i = 0; i < DATA_LENGTH ; i++){ //fill input data array with random 'garbage' values
        input[i] = mt_rand16();
    }
    
    FILE *fp;
    fp = fopen( "rand_file" , "w" );
    fwrite(input , 2, sizeof(input), fp );
    
    fclose(fp);

	return 0;
}
