#include "hash_table.h"

HashTable* HashTableCtor(uint32_t (*HashCalculate)(void* key), size_t number_of_lists)
{
    assert((HashCalculate != nullptr) && "Pointer to \'HashCalculate\' is NULL!!!\n");

    HashTable* new_hash_table = (HashTable*) calloc(1, sizeof(HashTable));
    assert((new_hash_table != nullptr) && "Program can not allocate memory!!!\n");

    new_hash_table->size = number_of_lists;

    new_hash_table->data = (List*) calloc(number_of_lists, sizeof(List));
    assert((new_hash_table->data != nullptr) && "Program can not allocate memory!!!\n");

    for (size_t i = 0; i < number_of_lists; i++)
    {
        ListCtor(new_hash_table->data + i, 1024); //!
    }

    new_hash_table->hash_func = HashCalculate;

    return new_hash_table;
}

void HashTableDtor(HashTable* const hash_table)
{
    assert((hash_table != nullptr) && "Pointer to \'hash_table\' is NULL!!!\n");

    for (size_t i = 0; i < hash_table->size; i++)
    {
        ListDtor(hash_table->data + i);
    }

    free(hash_table->data);

    hash_table->data        = nullptr;
    hash_table->size        = 0;
    hash_table->hash_func   = nullptr;

    free(hash_table);
}

void HashTableInsert(HashTable* const hash_table, void* const key)
{
    assert((hash_table != nullptr) && "Pointer to \'hash_table\' is NULL!!!\n");
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    uint32_t hash = hash_table->hash_func(key);

    ListPushBack(&(hash_table->data[hash]), key);
}

int HashTableSearch(const HashTable* const hash_table, void* const key)
{
    assert((hash_table != nullptr) && "Pointer to \'hash_table\' is NULL!!!\n");
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");

    uint32_t hash = hash_table->hash_func(key);

    return ListSearch(hash_table->data + hash, key);
}