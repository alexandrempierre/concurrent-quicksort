#ifndef RAND_INT_H
#define RAND_INT_H

#define LCG_RAND_MAX    0x7ffffffe
#define LCG_MULTIPLIER  48271
#define LCG_MODULUS     0x7fffffff

#include <stdint.h>

uint32_t lcg_parkmiller (uint32_t *state);

#endif