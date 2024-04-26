#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/*==================CONSTANTS=================*/

enum TextErrors
{
    TEXT_ERR_NO         = 0,
    TEXT_ERR_FILE_OPEN  = 1 << 0,
    TEXT_ERR_FILE_CLOSE = 1 << 1,
    TEXT_ERR_FILE_EMPTY = 1 << 2,
    TEXT_ERR_MEM_ALLOC  = 1 << 3,
    TEXT_ERR_FREAD      = 1 << 4,
};

const char* const MARTIN_EDEN_FILE  = "Data/MartinEden.txt";
const char* const CLEAN_FILE        = "Data/CleanFile.bin";

/*===================STRUCTS==================*/

struct Word
{
    const char* begin   = nullptr;
    size_t      size    = 0;
};

struct  Text
{
    char*   buffer              = nullptr;
    size_t  buffer_size         = 0;

    Word*   words_array         = nullptr;
    size_t  words_array_size    = 0;
};


/*==================FUNCTIONS=================*/

TextErrors  TextCtor(Text* text, const char* file_name);
TextErrors  TextDtor(Text* text);

TextErrors  TextPrint(Text* text, const char* file_name);

void        TextVerify(Text* text, TextErrors error);

/*============================================*/

#endif 