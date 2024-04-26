#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../../List/source/list.h"
#include "../../TextLib/source/text.h"
#include "hash_func.h"

/*==================CONSTANTS=================*/

enum HashTableErrors
{
    HASH_ERR_NO                 = 0,
    HASH_ERR_MEM_ALLOC          = 1 << 0,
    HASH_ERR_LIST_INVALID       = 1 << 1,
    HASH_ERR_TEXT_INVALID       = 1 << 2,
    HASH_ERR_SIZE_IS_NULL       = 1 << 3,
    HASH_ERR_HASH_FUNC_IS_NULL  = 1 << 4,
    HASH_ERR_DATA_IS_NULL       = 1 << 5,
    HASH_ERR_OPEN_FILE          = 1 << 6,
};

const size_t    HASH_TABLE_SIZE = 3049;

/*===================STRUCTS==================*/

struct HashTable
{
    List*       data;
    size_t      size;
    uint32_t (*hash_func)(const char* key, size_t len);
};

/*==================FUNCTIONS=================*/

HashTableErrors HashTableCtor(HashTable* const hash_table, uint32_t (*HashCalculate)(const char* key, size_t len), size_t number_of_lists);
HashTableErrors HashTableDtor(HashTable* const hash_table);

int             HashTableInsert(HashTable* const hash_table, const char* key, size_t len, error_t* error);

extern "C" int  FastHashTableSearch(const HashTable* const hash_table, const char* key, size_t len);

int             HashTableSearch(const HashTable* const hash_table, const char* key, size_t len, error_t* error);

error_t         HashTableVerify(HashTable* const hash_table);
void            HashTableErrorsPrint(error_t error);

error_t         AddTextToHashTable(HashTable* const hash_table, Text* const text);

error_t         PrintListSizes(HashTable* const hash_table, const char* file_name);

/*============================================*/

#endif