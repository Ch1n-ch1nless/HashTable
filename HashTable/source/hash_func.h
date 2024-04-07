#ifndef HASH_FUNC_H
#define HASH_FUNC_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

uint32_t    HashCalculate1(void* key);
uint32_t    HashCalculate2(void* key);
uint32_t    HashCalculate3(void* key);
uint32_t    HashCalculate4(void* key);
uint32_t    HashCalculate5(void* key);
uint32_t    HashCalculate6(void* key);
uint32_t    HashCalculate7(void* key);

#endif