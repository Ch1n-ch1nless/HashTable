#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include "../../TextLib/source/text.h"
#include "../../List/source/list.h"
#include "../../HashTable/source/hash_table.h"

//FLAGS
const char* const FLAG_TEST_HASH_FUNCTIONS  = "--test_func";
const char* const FLAG_TEST_HASH_SEARCH     = "--test_srch";
const char* const FLAG_HASH                 = "--function";
const char* const FLAG_FILE                 = "--file";


enum TestMode : int
{
    TEST_NO             = -1,
    TEST_HASH_SEARCH    =  0,
    TEST_HASH_FUNC      =  1,
};

struct TestConfig
{
    TestMode    test_mode;
    uint32_t    (*hash_func)(const char* key, size_t len);
    const char* file;
};

TestConfig ArgsParse(int argc, char* argv[]);

#endif