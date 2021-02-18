#include "lcg.h"

/* https://en.wikipedia.org/wiki/Lehmer_random_number_generator#Sample_C99_code */


/* This is a linear congruent generator, it's one of the simplest and fastest 
 * ways to generate random numbers. 
 * 
 * It's linear congruent because it takes the input multiplicates by a constant 
 * and gets the remainder of the division by another constant.
 * 
 * Input: a unsigned integer storing the state of the generator
 * Output: a pseudo-random number
 * */
uint32_t lcg_parkmiller (uint32_t *state) {
    return *state = (uint64_t)*state * LCG_MULTIPLIER % LCG_MODULUS;
}