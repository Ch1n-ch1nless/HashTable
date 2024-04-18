#include "hash_func.h"

uint32_t HashReturnConst(const char* key, size_t len)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((len != 0)       && "Len is invalid!!!\n");

    return 0;
}

uint32_t HashReturnFirstASCIICode(const char* key, size_t len)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((len != 0)       && "Len is invalid!!!\n");

    return *(key);
}

uint32_t HashReturnStrlen(const char* key, size_t len)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((len != 0)       && "Len is invalid!!!\n");

    return len;
}

uint32_t HashReturnSumCodes(const char* key, size_t len)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((len != 0)       && "Len is invalid!!!\n");

    uint32_t control_sum = 0;

    for (int i = 0; i < len; i++)
    {
        control_sum += *((char*)key + i);
    }

    return control_sum;
}

inline static uint32_t RORCalculate(uint32_t hash)
{
    return hash >> 1 | hash << 31;
}

uint32_t HashRorFunction(const char* key, size_t len)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((len != 0)       && "Len is invalid!!!\n");

    uint32_t hash   = 0;

    for (int i = 0; i < len; i++)
    {
        //Make cyclic shift right
        hash = RORCalculate(hash);
        hash ^= key[i];
    }

    return hash;
}

inline static uint32_t ROLCalculate(uint32_t hash)
{
    return hash << 1 | hash >> 31;
}

uint32_t HashRolFunction(const char* key, size_t len)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((len != 0)       && "Len is invalid!!!\n");

    uint32_t hash   = 0;

    for (int i = 0; i < len; i++)
    {
        //Make cyclic shift left
        hash = ROLCalculate(hash);
        hash ^= key[i];
    }

    return hash;
}

uint32_t HashCrc32(const char* key, size_t len)
{
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((len != 0)       && "Len is invalid!!!\n");

    uint_least32_t crc = 0xFFFFFFFF;
    while (len--)
        crc = (crc >> 8) ^ Crc32Table[(crc ^ *key++) & 0xFF];
    
    return crc ^ 0xFFFFFFFF;
}

uint32_t HashFastCrc32(const char* key, size_t len)
{
    uint32_t crc = 0xFFFFFFFF;

    asm (
        ".intel_syntax noprefix\n\t"
        "movzx  edx, BYTE PTR [%1]\n\t"
        "test    dl, dl\n\t"
        "je      .end_of_cycle\n\t"
        "add     %1, 1\n\t"
        "mov     %0, -1\n\t"
        ".next_char:\n\t"
        "add     %1, 1\n\t"
        "crc32   %0, dl\n\t"
        "movzx   edx, BYTE PTR [%1-1]\n\t"
        "test    dl, dl\n\t"
        "jne     .next_char\n\t"
        ".end_of_cycle:\n\t"
        "not     %0\n\t"
        ".att_syntax\n\t"
        : "=r"(crc)
        : "r"(key)
        : "edx"
    );

    return crc;
}

