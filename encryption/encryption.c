#include <stdio.h>
#include <stdlib.h>
#include "gf.h"
#include "mt64.h"

#define SPACE	640000
#define DATA_LENGTH   SPACE / sizeof(uint16_t)

//Sorry for global variables...
uint16_t	*input; // array of input data
uint16_t	*output; // array of output data
uint16_t	*coef; //random coefficients for Random encoding

int		i, j;
uint64_t	rand_init[4] = {UINT64_C(0xfd308), UINT64_C(0x65ab8), UINT64_C(0x931cd54), UINT64_C(0x9475ea2)};

uint16_t mt_rand16()
{
    return (uint16_t)genrand64_int64();
}

void allocate(){
    
    // Allocate
	if ((input = (uint16_t *)malloc(SPACE * 2)) == NULL) { // 16bit
		perror("malloc");
		exit(1);
	}
	output = input + (SPACE / sizeof(uint16_t)); // 16bit
	coef = malloc(sizeof(uint16_t) * 3);
}

void inputRandomNumbers(){
	for (i = 0; i < SPACE /sizeof(uint16_t); i++) //fill input data array with random 'garbage' values
	{
		input[i] = mt_rand16();
	}
    
    for (i = 0; i < 3; i++)
        coef[i] = mt_rand16();
}

//
void galoisField(){
    //    uint16_t    a[3][3], b[3], x[3];
    //b[0] = a[0][0] * x[0] + a[0][1] * x[1] + a[0][2] * x[2];
    //b[1] = a[1][0] * x[0] + a[1][1] * x[1] + a[1][2] * x[2];
    //b[2] = a[2][0] * x[0] + a[2][1] * x[1] + a[2][2] * x[2];
    
    
    for(i = 0 ; i < DATA_LENGTH ; i += 3){
        for(j = 0; j < 3; j++){
            //This thing needs to be written into actual code
            //output[j] = GF16mul(input[j][0], coef[0]) ^ GF16mul(input[j][1], coef[1]) ^ GF16mul(input[j][2], coef[2]);
            output[j] = GF16mul(input[i], coef[0]) ^ GF16mul(input[i+1], coef[1]) ^ GF16mul(input[i+2], coef[2]);
        }
    }
    
}

int main(int argc, char **argv){
    
    // Initialize GF
	GF16init();
    
    allocate();
    
    inputRandomNumbers();
    
    galoisField();
    
    //do something with output
    return 1;
}