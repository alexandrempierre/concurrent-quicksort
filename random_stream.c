#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lcg.h"

int main (int argc, char const *argv[]) {
    uint32_t seed = (uint64_t) time(NULL) % 0x7fffffff;
    uint32_t upper_bound = LCG_RAND_MAX;

    seed = seed ? seed == 0x7fffffff ? 0x7ffffffe : seed : 1;

    if (argc > 1) {
        sscanf(argv[1], "%u", &seed);
    }

    if (argc > 2) {
        sscanf(argv[2], "%u", &upper_bound);
    }

    while (1)
        printf( "%d\n", lcg_parkmiller(&seed) % upper_bound );
    
    return 0;
}