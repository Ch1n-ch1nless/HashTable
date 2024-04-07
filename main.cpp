#include "TextLib/source/text.h"

int main()
{
    Text text = {};

    TextErrors error = TextCtor(&text, MARTIN_EDEN_FILE);
    TextVerify(&text, error);

    error = TextPrint(&text, CLEAN_FILE);
    TextVerify(&text, error);

    error = TextDtor(&text);
    TextVerify(&text, error);

    return 0;
}