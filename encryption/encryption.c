#include <stdio.h>
#include <stdlib.h>
#include "gf.h"
#include "mt64.h"

#define SPACE  600000
#define DATA_LENGTH   SPACE / sizeof(uint16_t)

struct chunk{
    uint16_t a1, a2, a3;
    uint16_t *output;
};

uint16_t mt_rand16() {
    //uint16_t temp = genrand64_int64() & 0xffff;
    return genrand64_int64() & 0xffff;
}

int main(int argc, char **argv){
    // Initialize GF
    GF16init();
    
    //set up of variables
    uint16_t *input;

    uint16_t *output1;
    uint16_t *output2;
    uint16_t *output3;

    uint16_t coef[3][3];

    int i, j, t;
    //uint64_t rand_init[4] = {UINT64_C(0xfd308), UINT64_C(0x65ab8), UINT64_C(0x931cd54), UINT64_C(0x9475ea2)};

    // Allocate
    //if ((input = (uint16_t *)malloc(SPACE * 2)) == NULL) { // 16bit
    //if ((input = (uint16_t *)malloc(SPACE * 4)) == NULL) { // 16bit
    if ((input = malloc(DATA_LENGTH * sizeof *input)) == NULL) { // 16bit
        perror("malloc");
        exit(1);
    }
/*
    output1 = input + DATA_LENGTH; // 16bit
    output2 = output1 + DATA_LENGTH; // 16bit
    output3 = output2 + DATA_LENGTH; // 16bit
*/
    
    if ((output1 = malloc(DATA_LENGTH * sizeof *output1 * 3)) == NULL) { // 16bit
        perror("malloc");
        exit(1);
    }
    output2 = output1 + DATA_LENGTH; // 16bit
    output3 = output2 + DATA_LENGTH; // 16bit
    
    for (i = 0; i < DATA_LENGTH ; i++){ //fill input data array with random 'garbage' values
        input[i] = mt_rand16();
    }

    for (i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            //make sure coefficients aren't 0
            while(1) {
                coef[i][j] = mt_rand16();
                if(coef[i][j] != 0)
                    break;
            }
        }
    }

    //    uint16_t    a[3][3], b[3], x[3];
    //b[0] = a[0][0] * x[0] + a[0][1] * x[1] + a[0][2] * x[2];
    //b[1] = a[1][0] * x[0] + a[1][1] * x[1] + a[1][2] * x[2];
    //b[2] = a[2][0] * x[0] + a[2][1] * x[1] + a[2][2] * x[2];

    printf("Encoding\n");
    
    //encode section
    for(i = 0, t = 0; i < DATA_LENGTH ; t++, i += 3){
        //for(j = 0; j < 3; j++){
        //output1[t] = GF16mul(input[i], coef[0][0]) ^ GF16mul(input[i+1], coef[0][1]) ^ GF16mul(input[i+2], coef[0][2]);
        //output2[t] = GF16mul(input[i], coef[1][0]) ^ GF16mul(input[i+1], coef[1][1]) ^ GF16mul(input[i+2], coef[1][2]);
        //output3[t] = GF16mul(input[i], coef[2][0]) ^ GF16mul(input[i+1], coef[2][1]) ^ GF16mul(input[i+2], coef[2][2]);
        output1[t] = GF16mul(coef[0][0], input[i]) ^ GF16mul(coef[0][1], input[i+1]) ^ GF16mul(coef[0][2], input[i+2]);
        output2[t] = GF16mul(coef[1][0], input[i]) ^ GF16mul(coef[1][1], input[i+1]) ^ GF16mul(coef[1][2], input[i+2]);
        output3[t] = GF16mul(coef[2][0], input[i]) ^ GF16mul(coef[2][1], input[i+1]) ^ GF16mul(coef[2][2], input[i+2]);
        //}
    }
    //printf("%d and %d\n",t,i);

    //return 0;
    
    //do decode here
    printf("Decoding\n");
    
    
    //variables
    uint16_t *final;
    
    if ((final = malloc(DATA_LENGTH * sizeof *input)) == NULL) { // 16bit
        perror("malloc");
        exit(1);
    }
    
    uint16_t c0;
    uint16_t c1;
    uint16_t c2;
    uint16_t c3;
    uint16_t c4;
    
    /*
    c0 = coef[1][0] * coef[0][1] + coef[0][0] * coef[1][1];
    c1 = coef[1][0] * coef[0][2] + coef[0][0] * coef[1][2];
    c2 = coef[2][0] * coef[0][1] + coef[0][0] * coef[2][1];
    c3 = coef[2][0] * coef[0][2] + coef[0][0] * coef[2][2];
    c4 = c1 * c2 + c0 * c3;
     */
    
    c0 = GF16mul(coef[1][0], coef[0][1]) ^ GF16mul(coef[0][0], coef[1][1]);
    c1 = GF16mul(coef[1][0], coef[0][2]) ^ GF16mul(coef[0][0], coef[1][2]);
    c2 = GF16mul(coef[2][0], coef[0][1]) ^ GF16mul(coef[0][0], coef[2][1]);
    c3 = GF16mul(coef[2][0], coef[0][2]) ^ GF16mul(coef[0][0], coef[2][2]);
    c4 = GF16mul(c1, c2) ^ GF16mul(c0, c3);
    
    uint16_t *t0 = malloc(DATA_LENGTH/3 * sizeof *t0);
    uint16_t *t1 = malloc(DATA_LENGTH/3 * sizeof *t1);
    
    for(i = 0 ; i < DATA_LENGTH/3 ; i++){
        //t0[i] = coef[1][0] * output1[i] + coef[0][0] * output2[i];
        //t1[i] = coef[2][0] * output1[i] + coef[0][0] * output3[i];
        t0[i] = GF16mul(coef[1][0], output1[i]) ^ GF16mul(coef[0][0], output2[i]);
        t1[i] = GF16mul(coef[2][0], output1[i]) ^ GF16mul(coef[0][0], output3[i]);
    }
    
    uint16_t *x1 = malloc(DATA_LENGTH/3 * sizeof *x1);
    uint16_t *x2 = malloc(DATA_LENGTH/3 * sizeof *x2);
    uint16_t *x3 = malloc(DATA_LENGTH/3 * sizeof *x3);
    
    for(i = 0 ; i < DATA_LENGTH/3 ; i++){
        //x2[i] = (c2 * t0[i] + c0 * t1[i])/c4;
        //x1[i] = (t0[i] + c1 * x2[i])/c0;
        //x3[i] = (output1[i] + coef[0][1] * x1[i] + coef[0][2] * x2[i])/coef[0][0];
        x2[i] = GF16div((GF16mul(c2, t0[i]) ^ GF16mul(c0, t1[i])),c4);
        x1[i] = GF16div((t0[i] ^ GF16mul(c1, x2[i])),c0);
        x3[i] = GF16div((output1[i] ^ GF16mul(coef[0][1], x1[i]) ^ GF16mul(coef[0][2], x2[i])),coef[0][0]);
    }
    
    for(i = 0, t = 0 ; i < DATA_LENGTH ; i +=3, t++){
        //final[i] = x1[t];
        //final[i+1] = x2[t];
        //final[i+2] = x3[t];
        final[i] = x3[t];
        final[i+1] = x1[t];
        final[i+2] = x2[t];
    }
    
    printf("Checking...\n");
    
    //check
    for( i = 0 ; i < DATA_LENGTH ; i++){
        if(input[i] != final[i]){
            printf("Not equal at %d\n", i);
            printf("%d and %d\n", input[i], final[i]);
            //printf("%d and %d\n", input[i+1], final[i+1]);
            //printf("%d and %d\n", input[i+2], final[i+2]);
            break;
        }
    }
    
    printf("Done\n");
    
    
    
    return 0;
}




