#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    const char* key;
    size_t      size;
} list_elem_t;
typedef int error_t;

#define MAX(a, b) (a < b) ? b : a
#define MIN(a, b) (a < b) ? a : b

/*==================CONSTANTS=================*/

enum ListErrors
{
    LIST_ERR_NO                 = 0,
    LIST_ERR_MEM_ALLOC          = 1 << 0,
    LIST_ERR_DATA_IS_NULL       = 1 << 1,
    LIST_ERR_NEXT_IS_NULL       = 1 << 2,
    LIST_ERR_PREV_IS_NULL       = 1 << 3,
    LIST_ERR_SIZE_CAPACITY      = 1 << 4,
    LIST_ERR_FREE_IS_INVALID    = 1 << 5,
};

const list_elem_t   POISON_VALUE       = {nullptr, 0};
const int           LIST_INVALID_INDEX = -666;

//! Fix the ListResizeUp to decrease this constant!!!
const size_t        START_CAPACITY      = 16384;

/*===================STRUCTS==================*/

struct List
{
    list_elem_t*    data     = nullptr;
    int*            next     = nullptr;
    int*            prev     = nullptr;

    int             free     = 0;

    size_t          size     = 0;
    size_t          capacity = 0;
};

/*==================FUNCTIONS=================*/

error_t ListCtor(List* const list, size_t capacity);
error_t ListDtor(List* const list);

int     ListInsert(List* const list, list_elem_t key, size_t index, error_t* error);
int     ListPushBack(List* const list, list_elem_t key, error_t* error);
int     ListSearch(List* const list, list_elem_t key, error_t* error);

error_t ListVerify(List* const list);
void    ListPrintErrors(error_t error);

/*============================================*/

#endif 