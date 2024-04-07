#include "text.h"

int main()
{
    Text text = {};

    printf("LOL!\n");
    TextErrors error = TextCtor(&text, MARTIN_EDEN_FILE);
    TextVerify(&text, error);
    printf("LOL!\n");

    error = TextPrint(&text, CLEAN_FILE);
    TextVerify(&text, error);
    printf("LOL!\n");

    error = TextDtor(&text);
    TextVerify(&text, error);

    return 0;
}