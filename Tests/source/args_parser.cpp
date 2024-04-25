#include "args_parser.h"

TestConfig ArgsParse(int argc, char* argv[])
{
    TestConfig config = {TEST_NO, nullptr, nullptr};

    uint32_t (*hash_functions[])(const char *, size_t) =
    {
        HashReturnConst, HashReturnFirstASCIICode, HashReturnStrlen, HashReturnSumCodes, HashRorFunction, HashRolFunction, HashFastCrc32, HashCrc32, FastHashCRC32
    };

    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], FLAG_TEST_HASH_FUNCTIONS))
        {   
            config.test_mode = TEST_HASH_FUNC;
        }
        else if (!strcmp(argv[i], FLAG_TEST_HASH_SEARCH))
        {
            config.test_mode = TEST_HASH_SEARCH;
        }
        else if (!strcmp(argv[i], FLAG_HASH))
        {
            i++;
            int index = *(argv[i]) - '0';
            if (index < 0 || index > 8)
            {
                printf("ERROR in argument parser!\n");
                break;
            }
        
            config.hash_func = hash_functions[index];
        }
        else if (!strcmp(argv[i], FLAG_FILE))
        {
            i++;
            config.file = argv[i];
        }
    }

    return config;
}