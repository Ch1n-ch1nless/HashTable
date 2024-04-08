#include "TextLib/source/text.h"
#include "List/source/list.h"
#include "HashTable/source/hash_table.h"

int main()
{
    Text text = {};

    TextErrors text_error = TextCtor(&text, CLEAN_FILE);
    TextVerify(&text, text_error);

    HashTable hash_table = {};

    error_t hash_error = HashTableCtor(&hash_table, HashCrc32, HASH_TABLE_SIZE);
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

    hash_error = PrintListSizes(&hash_table, "test.csv");
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