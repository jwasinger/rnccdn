#include <stdio.h>
#include <stdlib.h>
#include "gf.h"
#include "mt64.h"

#define SPACE  640000
#define DATA_LENGTH   SPACE / sizeof(uint16_t)

//Sorry for global variables...
uint16_t  *input;
uint16_t  *output;
uint16_t  coef[3][3];

int    i, j;
uint64_t  rand_init[4] = {UINT64_C(0xfd308), UINT64_C(0x65ab8), UINT64_C(0x931cd54), UINT64_C(0x9475ea2)};

uint16_t mt_rand16() {
    return (uint16_t)(genrand64_int64() & 0xffff);
}

void allocate(){
    
    // Allocate
  if ((input = (uint16_t *)malloc(SPACE * 2)) == NULL) { // 16bit
    perror("malloc");
    exit(1);
  }
  output = input + (SPACE / sizeof(uint16_t)); // 16bit
}

void inputRandomNumbers(){
  for (i = 0; i < SPACE /sizeof(uint16_t); i++) //fill input data array with random 'garbage' values
  {
    input[i] = mt_rand16();
  }

  for (i = 0; i < 3; i++)
  {
      for(j = 0; j < 3; j++)
      {
          //make sure coefficients aren't 0
          while(1) { 
            coef[i][j] = mt_rand16();
            if(coef[i][j] != 0)
              break;
          }
      }
  }
}

//
void galoisField(){
    //    uint16_t    a[3][3], b[3], x[3];
    //b[0] = a[0][0] * x[0] + a[0][1] * x[1] + a[0][2] * x[2];
    //b[1] = a[1][0] * x[0] + a[1][1] * x[1] + a[1][2] * x[2];
    //b[2] = a[2][0] * x[0] + a[2][1] * x[1] + a[2][2] * x[2];

    for(i = 0 ; i < DATA_LENGTH ; i += 3){
        for(j = 0; j < 3; j++){
            output[j] = GF16mul(input[i], coef[j][0]) ^ GF16mul(input[i+1], coef[j][1]) ^ GF16mul(input[i+2], coef[j][2]);
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
