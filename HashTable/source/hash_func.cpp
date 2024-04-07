#include "hash_func.h"

uint32_t HashCalculate1(void* key)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    return 0;
}

uint32_t HashCalculate2(void* key)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    return *((char*)key);
}

uint32_t HashCalculate3(void* key)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    return strlen((char*)key);
}

uint32_t HashCalculate4(void* key)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    uint32_t control_sum = 0;
    int     len          = strlen((char*)key);

    for (int i = 0; i < len; i++)
    {
        control_sum += *((char*)key + i);
    }

    return control_sum;
}

uint32_t HashCalculate5(void* key)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    uint32_t hash   = 0;
    int      len    = strlen((char*)key);

    for (int i = 0; i < len; i++)
    {
        //Make cyclic shift right
        hash = (hash >> 1) ^ (hash & 1) << 31;
        hash ^= *((char*)key + i);
    }

    return hash;
}

uint32_t HashCalculate6(void* key)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    uint32_t hash   = 0;
    int      len    = strlen((char*)key);

    for (int i = 0; i < len; i++)
    {
        //Make cyclic shift left
        hash = (hash << 1) ^ (hash & 0x80000000) >> 31;
        hash ^= *((char*)key + i);
    }

    return hash;
}

uint32_t HashCalculate7(void* key)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    //...

    return 0;
}

