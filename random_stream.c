#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char const *argv[])
{
    unsigned int seed = time(NULL);
    int upper_bound = RAND_MAX;

    if (argc > 1) {
        sscanf(argv[1], "%u", &seed);
    }

    if (argc > 2) {
        sscanf(argv[2], "%d", &upper_bound);
    }

    srand(seed);

    while (1) {
        printf( "%d\n", rand() % upper_bound );
    }
    
    return 0;
}