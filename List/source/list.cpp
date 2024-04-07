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

    if (list->free <= 0 || list->free > list->capacity) error |= LIST_ERR_FREE_IS_INVALID;

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
    list_elem_t* new_data_ptr = (list_elem_t*) calloc(new_capacity, sizeof(list_elem_t));
    if (new_data_ptr == nullptr)
    {
        error |= LIST_ERR_MEM_ALLOC;
    }
    else
    {
        memcpy(new_data_ptr, list->data, list->capacity);
        free(list->data);
        list->data = new_data_ptr;
    }

    //Realloc list->next to new memory area!
    int* new_next_ptr = (int*) calloc(new_capacity, sizeof(int));
    if (new_next_ptr == nullptr)
    {
        error |= LIST_ERR_MEM_ALLOC;
    }
    else
    {
        memcpy(new_next_ptr, list->next, list->capacity);
        free(list->next);
        list->next = new_next_ptr;
    }

    //Realloc list->prev to new memory area!
    int* new_prev_ptr = (int*) calloc(new_capacity, sizeof(int));
    if (new_prev_ptr == nullptr)
    {
        error |= LIST_ERR_MEM_ALLOC;
    }
    else
    {
        memcpy(new_prev_ptr, list->prev, list->capacity);
        free(list->prev);
        list->prev = new_prev_ptr;
    }

    //Destroy list, if realloc was not successful
    if (error != LIST_ERR_NO)
    {
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

int ListInsert(List* const list, list_elem_t* key, size_t index, error_t* error)
{
    assert((list != nullptr) && "Pointer to \'list\' is NULL!!!\n");
    assert((key  != nullptr) && "Pointer to \'key\'  is NULL!!!\n");

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


    list->data[free_index] = (list_elem_t)key;
    list->next[index]      = free_index;
    list->prev[free_index] = index;

    ret_index = free_index;

    list->size++;

    return ret_index;
}

//----------------------------------------------

int ListPushBack(List* const list, void* key, error_t* error)
{
    return ListInsert(list, key, TAIL, error);
}

//----------------------------------------------

int ListSearch(List* const list, void* key)
{
    int is_found = 0;

    for (int i = HEAD; i != TAIL; i = list->next[i])
    {
        if (strcmp(list->data[i], (char*)key) == 0)
        {
            is_found = 1;
            break;
        }
    }

    return is_found;
}

//----------------------------------------------