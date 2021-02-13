#include "lcg.h"

/* https://en.wikipedia.org/wiki/Lehmer_random_number_generator#Sample_C99_code */

uint32_t lcg_parkmiller (uint32_t *state) {
    return *state = (uint64_t)*state * LCG_MULTIPLIER % LCG_MODULUS;
}