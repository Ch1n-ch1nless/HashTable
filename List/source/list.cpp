#include "list.h"

//----------------------------------------------

error_t ListCtor(List* const list, size_t capacity)
{
    assert((list != nullptr) && "Pointer to \'list\' is NULL!!!\n");

    list->data = (list_elem_t*) calloc(capacity, sizeof(list_elem_t));

    list->next = (int*) calloc(capacity, sizeof(int));

    list->prev = (int*) calloc(capacity, sizeof(int));

    if (!list->data || !list->next || !list->next)
    {
        free(list->data);
        free(list->next);
        free(list->prev);

        return LIST_ERR_MEM_ALLOC;
    }

    list->next[0] = 0;
    list->prev[0] = 0;
    list->data[0] = POISON_VALUE;

    for (size_t i = 1; i < capacity; i++)
    {
        list->data[i] = POISON_VALUE;
        list->next[i] = i + 1;
        list->prev[i] = LIST_INVALID_INDEX;
    }

    list->free = 1;

    list->size = 0;
    list->capacity = capacity;

    return LIST_ERR_NO;
}

//----------------------------------------------

error_t ListDtor(List* const list)
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
    list->free      = 0;

    return LIST_ERR_NO;
}

//----------------------------------------------

error_t ListVerify(List* const list)
{
    assert((list != nullptr) && "Pointer to \'list\' is NULL!!!\n");

    error_t error = LIST_ERR_NO;

    if (list->data == nullptr)                          error |= LIST_ERR_DATA_IS_NULL;

    if (list->next == nullptr)                          error |= LIST_ERR_NEXT_IS_NULL;

    if (list->prev == nullptr)                          error |= LIST_ERR_PREV_IS_NULL;

    if (list->capacity < list->size)                    error |= LIST_ERR_SIZE_CAPACITY;

    if (list->free < 0 || list->free > list->capacity)  error |= LIST_ERR_FREE_IS_INVALID;

    if (error != LIST_ERR_NO)
    {
        ListDtor(list);
    }

    return error;
}

//----------------------------------------------

void ListPrintErrors(error_t error)
{
    if (error & LIST_ERR_DATA_IS_NULL)
    {
        fprintf(stderr, "ERROR! \'list->data\' is NULL!!!\n");
    }
    if (error & LIST_ERR_NEXT_IS_NULL)
    {
        fprintf(stderr, "ERROR! \'list->next\' is NULL!!!\n");
    }
    if (error & LIST_ERR_PREV_IS_NULL)
    {
        fprintf(stderr, "ERROR! \'list->prev\' is NULL!!!\n");
    }
    if (error & LIST_ERR_SIZE_CAPACITY)
    {
        fprintf(stderr, "ERROR! \'list->size\' is more than \'list->capacity\'!!!\n");
    }
    if (error & LIST_ERR_FREE_IS_INVALID)
    {
        fprintf(stderr, "ERROR! \'list->free\' equals INVALID INDEX!!!\n");
    }
    if (error & LIST_ERR_MEM_ALLOC)
    {
        fprintf(stderr, "ERROR! Program can not allocate memory!!!\n");
    }
}

//----------------------------------------------

static error_t ListResizeUp(List* const list)
{
    assert((list != nullptr) && "Pointer to \'list\' is NULL!!!\n");

    error_t error       = LIST_ERR_NO;
    size_t new_capacity = list->capacity * 2;

    //Realloc list->data to new memory area!
    list->data = (list_elem_t*) realloc(list->data, new_capacity * sizeof(list_elem_t));
    if (list->data == nullptr)
    {
        error |= LIST_ERR_MEM_ALLOC;
    }

    //Realloc list->next to new memory area!
    list->next = (int*) realloc(list->next, new_capacity * sizeof(int));
    if (list->next == nullptr)
    {
        error |= LIST_ERR_MEM_ALLOC;
    }

    //Realloc list->prev to new memory area!
    list->prev = (int*) realloc(list->prev, new_capacity * sizeof(int));
    if (list->prev == nullptr)
    {
        error |= LIST_ERR_MEM_ALLOC;
    }

    //Destroy list, if realloc was not successful
    if (error != LIST_ERR_NO)
    {
        printf("ERROR!\n");
        ListDtor(list);
        return error;
    }

    //Fill other elements
    for (size_t i = list->capacity; i < new_capacity; i++)
    {
        list->data[i] = POISON_VALUE;
        list->prev[i] = LIST_INVALID_INDEX;
        list->next[i] = i + 1;
    }

    list->capacity = new_capacity;

    return error;
}

//----------------------------------------------

#define HEAD list->next[0]
#define TAIL list->prev[0]

int ListInsert(List* const list, const char* key, size_t len, size_t index, error_t* error)
{
    assert((list    != nullptr) && "Pointer to \'list\' is NULL!!!\n");
    assert((key     != nullptr) && "Pointer to \'key\' is NULL!!!\n");
    assert((error   != nullptr) && "Pointer to \'error\' is NULL!!!\n");

    *error = ListVerify(list);
    if (*error != LIST_ERR_NO)
    {
        ListPrintErrors(*error);
        return LIST_INVALID_INDEX;
    }

    if (list->size == list->capacity)
    {
        *error = ListResizeUp(list);
        if (*error == LIST_ERR_MEM_ALLOC)
        {
            ListPrintErrors(*error);
            return LIST_INVALID_INDEX;
        }
    }

    int ret_index = LIST_INVALID_INDEX;

    int free_index = list->free;
    list->free     = list->next[free_index];

    list->next[free_index]             = list->next[index];
    list->prev[list->next[free_index]] = free_index;


    strncpy(list->data[free_index].key, key, len);
    list->data[free_index].size = len;

    list->next[index]      = free_index;
    list->prev[free_index] = index;

    ret_index = free_index;

    list->size++;

    return ret_index;
}

//----------------------------------------------

int ListPushBack(List* const list, const char* key, size_t len, error_t* error)
{
    return ListInsert(list, key, len, TAIL, error);
}

//----------------------------------------------

int ListSearch(List* const list,const char* key, size_t len, error_t* error)
{
    assert((list    != nullptr) && "Pointer to \'list\'     is NULL!!!\n");
    assert((key     != nullptr) && "Pointer to \'key\'      is NULL!!!\n");
    assert((error   != nullptr) && "Pointer to \'error\'    is NULL!!!\n");

    int index = LIST_INVALID_INDEX;

    for (int i = HEAD; i != 0; i = list->next[i])
    {
        if (strncmp(list->data[i].key, key, MAX(list->data[i].size, len)) == 0)
        {
            index = i;
            break;
        }
    }

    return index;
}

#undef HEAD
#undef TAIL

//----------------------------------------------