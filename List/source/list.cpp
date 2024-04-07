#include "list.h"

void ListCtor(List* const list, size_t capacity)
{
    assert((list != nullptr) && "Pointer to \'list\' is NULL!!!\n");

    list->data = (list_elem_t*) calloc(capacity, sizeof(list_elem_t));
    assert((list->data != nullptr) && "Program can not allocate memory!\n");

    list->next = (int*) calloc(capacity, sizeof(int));
    assert((list->next != nullptr) && "Program can not allocate memory!\n");

    list->prev = (int*) calloc(capacity, sizeof(int));
    assert((list->prev != nullptr) && "Program can not allocate memory!\n");

    for (size_t i = 0; i < capacity; i++)
    {
        list->data[i] = POISON_VALUE;
        list->next[i] = i + 1;
        list->prev[i] = LIST_INVALID_INDEX;
    }

    list->free = 1;

    list->size = 0;
    list->capacity = capacity;
}

void ListDtor(List* const list)
{
    assert((list != nullptr) && "Pointer to \'list\' is NULL!!!\n");

    free(list->data);
    list->data = nullptr;

    free(list->next);
    list->next = nullptr;

    free(list->prev);
    list->prev = nullptr;

    list->capacity  = 0;
    list->size      = 0;
}

size_t ListInsert(List* const list, void* key, size_t index)
{
    assert((list != nullptr) && "Pointer to \'list\' is NULL!!!\n");
    assert((key  != nullptr) && "Pointer to \'key\'  is NULL!!!\n");

    int ret_index = -1;

    int free_index = list->free;
    list->free     = list->next[free_index];

    list->next[free_index]             = list->next[index];
    list->prev[list->next[free_index]] = free_index;


    list->data[free_index] = (list_elem_t)key;
    list->next[index]      = free_index;
    list->prev[free_index] = index;

    ret_index = free_index;

    list->size++;

    return ret_index;
}

size_t ListPushBack(List* const list, void* key)
{
    return ListInsert(list, key, list->prev[0]);
}

int ListSearch(List* const list, void* key)
{
    int is_found = 0;

    for (int i = list->next[0]; i != list->prev[0]; i = list->next[i])
    {
        if (strcmp(list->data[i], (char*)key) == 0)
        {
            is_found = 1;
            break;
        }
    }

    return is_found;
}