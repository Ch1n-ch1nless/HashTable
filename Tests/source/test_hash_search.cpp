#include "test.h"
#include <time.h>
#include <immintrin.h>

void TestHashSearch(uint32_t (*HashFunction)(const char* key, size_t len))
{
    Text text = {};

    TextErrors text_error = TextCtor(&text, MARTIN_EDEN_FILE);
    TextVerify(&text, text_error);

    HashTable hash_table = {};

    error_t hash_error = HashTableCtor(&hash_table, HashFunction, HASH_TABLE_SIZE);
    if (hash_error != HASH_ERR_NO)
    {
        hash_error |= HashTableVerify(&hash_table);
        HashTableErrorsPrint(hash_error);
    }

    hash_error = AddTextToHashTable(&hash_table, &text);
    if (hash_error != HASH_ERR_NO)
    {
        hash_error |= HashTableVerify(&hash_table);
        HashTableErrorsPrint(hash_error);
    }

    printf("Turn ON timer\n");

    clock_t             clock_time_begin    = clock();
    unsigned long long  time_begin          = __rdtsc();

    for (int j = 0; j < 3000; j++)
    {
        for (size_t i = 0; i < text.words_array_size; i++)
        {
            HashTableSearch(&hash_table, text.words_array[i].begin, text.words_array[i].size, &hash_error);
            if (hash_error != HASH_ERR_NO)
            {
                hash_error |= HashTableVerify(&hash_table);
                HashTableErrorsPrint(hash_error);
            }
        }
    }

    clock_t             clock_time_end      = clock();
    unsigned long long  time_end            = __rdtsc();

    printf("Turn OFF timer\n");
    printf("Time in tacts: %lld\n", time_end - time_begin);
    printf("Time in seconds: %lg\n", ((double) (clock_time_end - clock_time_begin)) / CLOCKS_PER_SEC);

    text_error = TextDtor(&text);
    hash_error = HashTableDtor(&hash_table);
}