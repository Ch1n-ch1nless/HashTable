#include "TextLib/source/text.h"
#include "List/source/list.h"
#include "HashTable/source/hash_table.h"

void ArgsParser(int argc, char* argv[], size_t* index, const char** output_file)
{
    assert((argv != nullptr) && "Pointer to \'argv\' is NULL!!!\n");
    assert((argc == 3) && "\'argc\' does not equal 3!!!\n");

    *index          = (*argv[1] - '0');
    *output_file    = argv[2];
}

int main(int argc, char* argv[])
{
    size_t      index       = 0;
    const char* output_file = nullptr;

    ArgsParser(argc, argv, &index, &output_file);

    Text text = {};

    TextErrors text_error = TextCtor(&text, CLEAN_FILE);
    TextVerify(&text, text_error);

    uint32_t (*hash_functions[7])(const char *, size_t) =
    {
        HashReturnConst, HashReturnFirstASCIICode, HashReturnStrlen, HashReturnSumCodes, HashRorFunction, HashRolFunction, HashCrc32
    };

    HashTable hash_table = {};

    error_t hash_error = HashTableCtor(&hash_table, hash_functions[index], HASH_TABLE_SIZE);
    if (hash_error != HASH_ERR_NO)
    {
        hash_error |= HashTableVerify(&hash_table);
        HashTableErrorsPrint(hash_error);
        return -1;
    }

    hash_error = AddTextToHashTable(&hash_table, &text);
    if (hash_error != HASH_ERR_NO)
    {
        hash_error |= HashTableVerify(&hash_table);
        HashTableErrorsPrint(hash_error);
        return -2;
    }

    hash_error = PrintListSizes(&hash_table, output_file);
    if (hash_error != HASH_ERR_NO)
    {
        hash_error |= HashTableVerify(&hash_table);
        HashTableErrorsPrint(hash_error);
        return -3;
    }

    text_error = TextDtor(&text);
    TextVerify(&text, text_error);

    return 0;
}