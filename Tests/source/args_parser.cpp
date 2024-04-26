#include "args_parser.h"

TestConfig ArgsParse(int argc, char* argv[])
{
    TestConfig config = {TEST_NO, nullptr, nullptr};

    uint32_t (*hash_functions[])(const char *, size_t) =
    {
        HashReturnConst, HashReturnFirstASCIICode, HashReturnStrlen, HashReturnSumCodes, HashRorFunction, HashRolFunction, HashFastCrc32, HashCrc32, FastHashCRC32
    };

    for (int arg_id= 0; arg_id< argc; arg_id++)
    {
        if (!strcmp(argv[arg_id], FLAG_TEST_HASH_FUNCTIONS))
        {   
            config.test_mode = TEST_HASH_FUNC;
        }
        else if (!strcmp(argv[arg_id], FLAG_TEST_HASH_SEARCH))
        {
            config.test_mode = TEST_HASH_SEARCH;
        }
        else if (!strcmp(argv[arg_id], FLAG_HASH))
        {
            arg_id++;
            int index = *(argv[arg_id]) - '0';
            if (index < 0 || index > 8)
            {
                printf("ERROR in argument parser!\n");
                break;
            }
        
            config.hash_func = hash_functions[index];
        }
        else if (!strcmp(argv[arg_id], FLAG_FILE))
        {
            arg_id++;
            config.file = argv[arg_id];
        }
    }

    return config;
}