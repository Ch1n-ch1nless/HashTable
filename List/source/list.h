#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef char* list_elem_t;

/*============================================*/

const list_elem_t POISON_VALUE       = nullptr;
const int         LIST_INVALID_INDEX = -666;

struct List
{
    list_elem_t*    data     = nullptr;
    int*            next     = nullptr;
    int*            prev     = nullptr;

    size_t          free     = 0;

    size_t          size     = 0;
    size_t          capacity = 0;
};

/*============================================*/

void    ListCtor(List* const list, size_t capacity);
void    ListDtor(List* const list);

size_t  ListInsert(List* const list, void* key, size_t index);
size_t  ListPushBack(List* const list, void* key);
int     ListSearch(List* const list, void* key);

/*============================================*/

#endif 