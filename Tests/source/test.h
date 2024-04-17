#ifndef TEST_H
#define TEST_H

#include "../../TextLib/source/text.h"
#include "../../List/source/list.h"
#include "../../HashTable/source/hash_table.h"

void    TestHashFunction(uint32_t (*HashFunction)(const char* key, size_t len), const char* file_with_results);

void    TestHashSearch(uint32_t (*HashFunction)(const char* key, size_t len));

#endif