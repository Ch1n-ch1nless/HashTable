#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../../List/source/list.h"
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
};

const size_t    HASH_TABLE_SIZE = 1223;

/*===================STRUCTS==================*/

struct HashTable
{
    List*       data;
    size_t      size;
    uint32_t (*hash_func)(const char* key);
};

/*==================FUNCTIONS=================*/

HashTableErrors HashTableCtor(HashTable* const hash_table, uint32_t (*HashCalculate)(const char* key), size_t number_of_lists);
HashTableErrors HashTableDtor(HashTable* const hash_table);

int             HashTableInsert(HashTable* const hash_table, const char* key, error_t* error);
int             HashTableSearch(const HashTable* const hash_table, const char* key, error_t* error);

error_t         HashTableVerify(HashTable* const hash_table);
void            HashTableErrorsPrint(error_t error);

/*============================================*/

#endif