#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "list.h"
#include "hash_func.h"

/*============================================*/

struct HashTable
{
    List*       data;
    size_t      size;
    uint32_t (*hash_func)(void* key);
};

/*============================================*/

HashTable*  HashTableCtor(uint32_t (*HashCalculate)(void* key), size_t number_of_lists);
void        HashTableDtor(HashTable* const hash_table);

void        HashTableInsert(HashTable* const hash_table, void* const key);
int         HashTableSearch(const HashTable* const hash_table, void* const key);

/*============================================*/

#endif