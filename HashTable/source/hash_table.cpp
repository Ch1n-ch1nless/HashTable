#include "hash_table.h"

//----------------------------------------------

HashTableErrors HashTableCtor(HashTable* hash_table, uint32_t (*HashCalculate)(const char* key, size_t len), size_t number_of_lists)
{
    assert((HashCalculate != nullptr) && "Pointer to \'HashCalculate\' is NULL!!!\n");
    assert((hash_table != nullptr)    && "Pointer to \'hash_table\' is NULL!!!\n");

    error_t error = HASH_ERR_NO;

    hash_table->size = number_of_lists;

    hash_table->data = (List*) calloc(number_of_lists, sizeof(List));
    if (hash_table->data == nullptr)
    {
        return HASH_ERR_MEM_ALLOC;
    }

    for (size_t i = 0; i < number_of_lists; i++)
    {
        error  = ListCtor(hash_table->data + i, START_CAPACITY);
        if (error != LIST_ERR_NO)
        {
            ListPrintErrors(error);
            HashTableDtor(hash_table);
            return HASH_ERR_LIST_INVALID;
        }
    }

    hash_table->hash_func = HashCalculate;

    return HASH_ERR_NO;
}

//----------------------------------------------

HashTableErrors HashTableDtor(HashTable* const hash_table)
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

    return HASH_ERR_NO;
}

//----------------------------------------------

int HashTableInsert(HashTable* const hash_table, const char* key, size_t len, error_t* error)
{
    assert((hash_table != nullptr) && "Pointer to \'hash_table\' is NULL!!!\n");
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((error != nullptr) && "Pointer to \'error\' is NULL!!!\n");

    uint32_t hash = (hash_table->hash_func(key, len)) % hash_table->size;

    *error = LIST_ERR_NO;
    int index = LIST_INVALID_INDEX;
    int is_found = ListSearch(&(hash_table->data[hash]), key, len, error);

    if (*error != LIST_ERR_NO)
    {
        ListPrintErrors(*error);
        *error = HASH_ERR_LIST_INVALID;
        return LIST_INVALID_INDEX;
    }
    if (is_found == LIST_INVALID_INDEX)
    {
        index = ListPushBack(&(hash_table->data[hash]), key, len, error);
        if (*error != LIST_ERR_NO)
        {
            ListPrintErrors(*error);
            *error = HASH_ERR_LIST_INVALID;
            return LIST_INVALID_INDEX;
        }
    }

    return index;
}

//----------------------------------------------

int HashTableSearch(const HashTable* const hash_table, const char* key, size_t len, error_t* error)
{
    assert((hash_table != nullptr) && "Pointer to \'hash_table\' is NULL!!!\n");
    assert((key != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((error != nullptr) && "Pointer to \'error\' is NULL!!!\n");

    uint32_t hash = (hash_table->hash_func(key,len)) % hash_table->size;

    *error = LIST_ERR_NO;
    int is_found = ListSearch(hash_table->data + hash, key, len, error);

    if (*error != LIST_ERR_NO)
    {
        ListPrintErrors(*error);
        *error = HASH_ERR_LIST_INVALID;
        return 0;
    }

    return is_found;
}

//----------------------------------------------

error_t HashTableVerify(HashTable* const hash_table)
{
    assert((hash_table != nullptr) && "Pointer to \'hash_table\' is NULL!!!\n");

    error_t error = HASH_ERR_NO;

    if (hash_table->size == 0)              error |= HASH_ERR_SIZE_IS_NULL;
    if (hash_table->hash_func == nullptr)   error |= HASH_ERR_HASH_FUNC_IS_NULL;
    if (hash_table->data == nullptr)        error |= HASH_ERR_DATA_IS_NULL;

    if (hash_table->data != nullptr)
    {
        error_t list_error = LIST_ERR_NO;

        for (size_t i = 0; i < hash_table->size; i++)
        {
            error_t list_error = LIST_ERR_NO;
            list_error = ListVerify(&(hash_table->data[i]));
            if (list_error == LIST_ERR_NO)
            {
                ListPrintErrors(list_error);
                return error | HASH_ERR_LIST_INVALID;
            }
        }
    }

    return error;
}

//----------------------------------------------

void HashTableErrorsPrint(error_t error)
{
    if (error & HASH_ERR_DATA_IS_NULL)
    {
        fprintf(stderr, "ERROR! Hash table data is NULL!!!\n");
    }
    if (error & HASH_ERR_DATA_IS_NULL)
    {
        fprintf(stderr, "ERROR! Hash table size is NULL!!!\n");
    }
    if (error & HASH_ERR_HASH_FUNC_IS_NULL)
    {
        fprintf(stderr, "ERROR! Hash function is NULL!!!\n");
    }
    if (error & HASH_ERR_LIST_INVALID)
    {
        fprintf(stderr, "ERROR! List in Hash table is invalid!!!\n");
    }
    if (error & HASH_ERR_TEXT_INVALID)
    {
        fprintf(stderr, "ERROR! Text in Hash table is invalid!!!\n");
    }
    if (error & HASH_ERR_MEM_ALLOC)
    {
        fprintf(stderr, "ERROR! Program can not allocate memory!!!\n");
    }
    if (error & HASH_ERR_OPEN_FILE)
    {
        fprintf(stderr, "ERROR! Program can not open file!\n");
    }
}

//----------------------------------------------

error_t AddTextToHashTable(HashTable* const hash_table, Text* const text)
{
    assert((hash_table != nullptr) && "Pointer to \'hash_table\' is NULL!!!\n");
    assert((text != nullptr) && "Pointer to \'text\' is NULL!!!\n");

    error_t error = HASH_ERR_NO;

    for (size_t i = 0; i < text->words_array_size; i++)
    {
        HashTableInsert(hash_table, text->words_array[i].begin, text->words_array[i].size, &error);
        if (error != HASH_ERR_NO)
            return error;
    }

    return error;
}

//----------------------------------------------

error_t PrintListSizes(HashTable* const hash_table, const char* file_name)
{
    assert((hash_table != nullptr) && "Pointer to \'hash_table\' is NULL!!!\n");

    FILE* file_ptr = fopen(file_name, "wb");
    if (file_name == nullptr)
    {
        return HASH_ERR_OPEN_FILE;
    }

    size_t sum = 0;

    for (size_t i = 0; i < hash_table->size; i++)
    {
        fprintf(file_ptr, "%lu, %lu\n", i, hash_table->data[i].size);
        sum += hash_table->data[i].size;
    }

    return HASH_ERR_NO;
}

//----------------------------------------------