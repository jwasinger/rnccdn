#include <stdio.h>
#include <stdlib.h>

// #include <assert.h>
// google test...

#include "gf.h"
#include "mt64.h"

#define SPACE  600000
//#define DATA_LENGTH   SPACE / sizeof(uint16_t)

//
//
//
struct arguments {
    int num_of_chunks;
    char * input_file_name;
};

//
//
//
struct chunk {
    //uint16_t a1, a2, a3;
    uint16_t coef[3];
    uint16_t *output;
    int numEmpty; //number of 16bit 0s at end of final file
};

//
// lksd;lk
//
uint16_t
mt_rand16(void)
{
    //uint16_t temp = genrand64_int64() & 0xffff;
    return genrand64_int64() & 0xffff;
    
} // mt_rand16()...

//
// getopts...
//
void
parse_args(int argc,
           char *argv[],
           struct arguments *facts
           )
{
    
} // parse_args()...

//
//
//
void
encodeFile(char **argv, // Parse everything in main or a separate function to parse and put tested inputs in a struct of "facts".
           struct chunk *output //
           )
{
    uint16_t *input = NULL;
    int DATA_LENGTH = 0;
    int i, x, t;
    
    //get file input
    FILE *fp;
    fp = fopen(argv[1], "r");
    
    if(fp != NULL){
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }
            
            /* Allocate our buffer to that size. */
            input = malloc(sizeof(uint16_t) * (bufsize));
            
            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }
            
            /* Read the entire file into memory. */
            size_t newLen = fread(input, sizeof(uint16_t), bufsize, fp);
            
            if (newLen == 0) {
                fputs("Error reading file", stderr);
            }/* else {
                source[newLen++] = '\0'; //Just to be safe.
            }*/
            DATA_LENGTH = sizeof(input)/sizeof(uint16_t);
        }
    }
    
    fclose(fp);
    
    //make coefficients
    for(x = 0 ; x < atoi(argv[2]) ; x++){
        struct chunk out;
        output[x] = out;
        
        if ((output[x].output = malloc(DATA_LENGTH * sizeof *output[x].output)) == NULL) { // 16bit
            perror("malloc");
            exit(1);
        }
    }
    //encode section
    for(i = 0, t = 0; i < DATA_LENGTH ; t++, i += 3){
        for(x = 0 ; x < atoi(argv[2]) ; x++){
            output[x].output[t] = GF16mul(output[x].coef[0], input[i]) ^ GF16mul(output[x].coef[1], input[i+1]) ^ GF16mul(output[x].coef[2], input[i+2]);
        }
        /*
        output[0].output[t] = GF16mul(output[0].coef[0], input[i]) ^ GF16mul(output[0].coef[1], input[i+1]) ^ GF16mul(output[0].coef[2], input[i+2]);
        output[1].output[t] = GF16mul(output[1].coef[0], input[i]) ^ GF16mul(output[1].coef[1], input[i+1]) ^ GF16mul(output[1].coef[2], input[i+2]);
        output[2].output[t] = GF16mul(output[2].coef[0], input[i]) ^ GF16mul(output[2].coef[1], input[i+1]) ^ GF16mul(output[2].coef[2], input[i+2]);
        */
    }
}

//
// Use getopts()
//
int main(int argc, char **argv){
    //input format
    //no input for random input
    //1 argument that is a file and number of chunks to create
    //3 arguments that are the chunks to use to create a file
    //check inout
    int state;
    if(argc == 1){
        //0 argument
        //random input
        state = 0;
    } else if(argc == 3){
        //2 arguments
        //file input
        //number of chunks to make
        state = 1;
    } else if(argc == 4){
        //3 arguments
        //3 chunks to use in creating file
        state = 2;
    } else {
        //too many arguments
        printf("Invalid input. Valid arguments:\n");
        printf("No arguments for random input.\nFilename and number of chunks to make.\nOr three files to reassemble\n");
        return 0;
    }
    
    // Initialize GF
    GF16init();
    
    //set up of variables
    //intput
    uint16_t *input;
    
    //output chunks
    struct chunk output[3];
    struct chunk out1;
    struct chunk out2;
    struct chunk out3;
    
    output[0] = out1;
    output[1] = out2;
    output[2] = out3;

    //loop variables
    int i, j, t;
    
    //data length variable
    int DATA_LENGTH;

    // Allocate size for input and output
    //check based on state
    if( state == 0 ){
        //No file. So random file
        DATA_LENGTH = SPACE / sizeof(uint16_t);
        //input allocation
        if ((input = malloc(DATA_LENGTH * sizeof *input)) == NULL) { // 16bit
            perror("malloc");
            exit(1);
        }
        //fill input with random numbers
        for (i = 0; i < DATA_LENGTH ; i++){
            input[i] = mt_rand16();
        }
        //output allocation
        if ((output[0].output = malloc(DATA_LENGTH * sizeof *output[0].output)) == NULL) { // 16bit
            perror("malloc");
            exit(1);
        }
        if ((output[1].output = malloc(DATA_LENGTH * sizeof *output[1].output)) == NULL) { // 16bit
            perror("malloc");
            exit(1);
        }
        if ((output[2].output = malloc(DATA_LENGTH * sizeof *output[2].output)) == NULL) { // 16bit
            perror("malloc");
            exit(1);
        }
    } else if(state == 1){
        //file input
        //file io operations here
        //...
        encodeFile(argv, output);
    } else if(state == 2){
        //3 files input for reassembly
    }
    
    //get coefficients (new)
    for (i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            //make sure coefficients aren't 0
            while(1) {
                output[i].coef[j] = mt_rand16();
                if(output[i].coef[j] != 0)
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
        output[0].output[t] = GF16mul(output[0].coef[0], input[i]) ^ GF16mul(output[0].coef[1], input[i+1]) ^ GF16mul(output[0].coef[2], input[i+2]);
        output[1].output[t] = GF16mul(output[1].coef[0], input[i]) ^ GF16mul(output[1].coef[1], input[i+1]) ^ GF16mul(output[1].coef[2], input[i+2]);
        output[2].output[t] = GF16mul(output[2].coef[0], input[i]) ^ GF16mul(output[2].coef[1], input[i+1]) ^ GF16mul(output[2].coef[2], input[i+2]);
    }
    
    
    printf("Decoding\n");
    
    //variables
    uint16_t *final;
    
    //allocate space for final file
    if ((final = malloc(DATA_LENGTH * sizeof *input)) == NULL) { // 16bit
        perror("malloc");
        exit(1);
    }
    
    //set up c variables
    uint16_t c0;
    uint16_t c1;
    uint16_t c2;
    uint16_t c3;
    uint16_t c4;
    
    //do math for c variables
    c0 = GF16mul(output[1].coef[0], output[0].coef[1]) ^ GF16mul(output[0].coef[0], output[1].coef[1]);
    c1 = GF16mul(output[1].coef[0], output[0].coef[2]) ^ GF16mul(output[0].coef[0], output[1].coef[2]);
    c2 = GF16mul(output[2].coef[0], output[0].coef[1]) ^ GF16mul(output[0].coef[0], output[2].coef[1]);
    c3 = GF16mul(output[2].coef[0], output[0].coef[2]) ^ GF16mul(output[0].coef[0], output[2].coef[2]);
    c4 = GF16mul(c1, c2) ^ GF16mul(c0, c3);
    
    uint16_t *t0 = malloc(DATA_LENGTH/3 * sizeof *t0);
    uint16_t *t1 = malloc(DATA_LENGTH/3 * sizeof *t1);
    
    for(i = 0 ; i < DATA_LENGTH/3 ; i++){
        t0[i] = GF16mul(output[1].coef[0], output[0].output[i]) ^ GF16mul(output[0].coef[0], output[1].output[i]);
        t1[i] = GF16mul(output[2].coef[0], output[0].output[i]) ^ GF16mul(output[0].coef[0], output[2].output[i]);
    }
    
    uint16_t *x1 = malloc(DATA_LENGTH/3 * sizeof *x1);
    uint16_t *x2 = malloc(DATA_LENGTH/3 * sizeof *x2);
    uint16_t *x3 = malloc(DATA_LENGTH/3 * sizeof *x3);
    
    for(i = 0 ; i < DATA_LENGTH/3 ; i++){
        x2[i] = GF16div((GF16mul(c2, t0[i]) ^ GF16mul(c0, t1[i])),c4);
        x1[i] = GF16div((t0[i] ^ GF16mul(c1, x2[i])),c0);
        x3[i] = GF16div((output[0].output[i] ^ GF16mul(output[0].coef[1], x1[i]) ^ GF16mul(output[0].coef[2], x2[i])),output[0].coef[0]);
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
    
    
    free(input);
    free(output[0].output);
    free(output[1].output);
    free(output[2].output);
    free(final);
    free(x1);
    free(x2);
    free(x3);
    
    return 0;
}




