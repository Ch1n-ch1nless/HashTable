#include "test.h"

void TestHashFunction(uint32_t (*HashFunction)(const char* key, size_t len), const char* file_with_results)
{
    assert(HashFunction);

    Text text = {};

    TextErrors text_error = TextCtor(&text, MARTIN_EDEN_FILE);
    TextVerify(&text, text_error);

    HashTable hash_table = {};

    error_t hash_error = HashTableCtor(&hash_table, HashFunction, HASH_TABLE_SIZE);
    if (hash_error != HASH_ERR_NO)
    {
        hash_error |= HashTableVerify(&hash_table);
        HashTableErrorsPrint(hash_error);
        assert(0);
    }

    hash_error = AddTextToHashTable(&hash_table, &text);
    if (hash_error != HASH_ERR_NO)
    {
        hash_error |= HashTableVerify(&hash_table);
        HashTableErrorsPrint(hash_error);
        assert(0);
    }

    hash_error = PrintListSizes(&hash_table, file_with_results);
    if (hash_error != HASH_ERR_NO)
    {
        hash_error |= HashTableVerify(&hash_table);
        HashTableErrorsPrint(hash_error);
        assert(0);
    } 

    text_error = TextDtor(&text);
    hash_error = HashTableDtor(&hash_table);
}