#include "test.h"
#include "args_parser.h"

int main(int argc, char* argv[])
{
    TestConfig config = ArgsParse(argc, argv);

    switch (config.test_mode)
    {
        case TEST_HASH_FUNC:
        {
            //printf("%s\n", config.file);
            TestHashFunction(config.hash_func, config.file);
            break;
        }
        case TEST_HASH_SEARCH:
        {
            TestHashSearch(config.hash_func);
            break;
        }
        case TEST_NO:
        default:
        {
            printf("ERROR! Incorrect command argument\n");
            break;
        }
    }

    return 0;
}