#include "text.h"

static TextErrors OpenFile(const char* file_name, FILE** file, const char* mode)
{
    assert(file_name);
    assert(file);
    assert(mode);

    *file = fopen(file_name, mode);
    if (*file == nullptr)
        return TEXT_ERR_FILE_OPEN;

    return TEXT_ERR_NO;
}

static TextErrors CloseFile(FILE* file)
{
    assert(file);

    if (fclose(file) != 0)
        return TEXT_ERR_FILE_CLOSE;

    return TEXT_ERR_NO;
}

/*============================================*/

static TextErrors ReadBufferFromFile(FILE* const file_ptr, Text* const text)
{
    assert(file_ptr);
    assert(text);

    text->buffer = (char*) calloc(text->buffer_size, sizeof(char));
    if (text->buffer == nullptr)
    {
        return TEXT_ERR_MEM_ALLOC;
    }

    size_t number_of_symbols = fread(text->buffer, sizeof(char), text->buffer_size, file_ptr);

    if (number_of_symbols != text->buffer_size) 
    {
        if (feof(file_ptr)) 
        {
            fprintf(stderr, "Error reading %s: unexpected end of file\n", "<STRING>");
            return TEXT_ERR_FREAD;
        } 
        else if (ferror(file_ptr)) 
        {
            fprintf(stderr, "Error reading %s", "<STRING>");
            return TEXT_ERR_FREAD;
        }
        
        if (number_of_symbols > text->buffer_size) 
        {
            printf("Error! Symbols in file  more then buffer size!");
            assert(false);
        }
    }

    return TEXT_ERR_NO;
}

static TextErrors CountBufferSize(const char* file_name, size_t* buffer_size)
{
    assert(file_name);
    assert(buffer_size);

    struct stat st = {};

    stat(file_name, &st);

    if (st.st_size == 0)
    {
        return TEXT_ERR_FILE_EMPTY;
    }

    *buffer_size = st.st_size;

    return TEXT_ERR_NO;
}

static TextErrors BufferCtor(Text* text, const char* file_name)
{
    assert(text);
    assert(file_name);

    TextErrors error = TEXT_ERR_NO;

    error = CountBufferSize(file_name, &(text->buffer_size));
    if (error != TEXT_ERR_NO)
    {
        return error;
    }

    FILE* infile_ptr = nullptr;
    error = OpenFile(file_name, &infile_ptr, "rb");
    if (error != TEXT_ERR_NO)
    {
        return error;
    }

    error = ReadBufferFromFile(infile_ptr, text);
    if (error != TEXT_ERR_NO)
    {
        return error;
    }

    error = CloseFile(infile_ptr);
    if (error != TEXT_ERR_NO)
    {
        return error;
    }

    return error;
}

static TextErrors BufferDtor(Text* text)
{
    assert(text);

    free(text->buffer);
    text->buffer = nullptr;

    text->buffer_size = 0;

    return TEXT_ERR_NO;
} 

/*============================================*/

static TextErrors WordsArrayCtor(Text* text)
{
    assert(text);

    TextErrors error = TEXT_ERR_NO;

    size_t number_of_words = 0;

    for (const char* cur = text->buffer; *cur != '\0'; number_of_words++)
    {
        while (isspace(*cur) || ispunct(*cur))
        {
            cur++;
        }

        if (!*cur) break;

        while (*cur && !isspace(*cur) && !ispunct(*cur))
        {
            cur++;
        }
    }

    if (number_of_words == 0)
    {
        fprintf(stderr, "No words in file!!!\n");
        return TEXT_ERR_FILE_EMPTY;
    }

    text->words_array_size = number_of_words;

    text->words_array = (Word*) calloc(number_of_words, sizeof(Word));
    if (text->words_array == nullptr)
    {
        return TEXT_ERR_MEM_ALLOC;
    }


    size_t word_iter = 0;
    const char* word_beg = nullptr;

    for (const char* cur = text->buffer; *cur != '\0'; word_iter++)
    {
        while (isspace(*cur) || ispunct(*cur) || *cur == '\n')
        {
            cur++;
        }

        if (!*cur) break;

        text->words_array[word_iter].begin = cur;
        word_beg = cur;
        
        while (*cur && !isspace(*cur) && !ispunct(*cur) && *cur != '\n')
        {
            cur++;
        }

        text->words_array[word_iter].size  = cur - word_beg;
    }

    return error;
}

/*============================================*/

TextErrors TextCtor(Text* text, const char* file_name)
{
    assert((text        != nullptr) && "Pointer to \'text\'      is NULL!!!\n");
    assert((file_name   != nullptr) && "Pointer to \'file_name\' is NULL!!!\n");

    TextErrors error = BufferCtor(text, file_name);
    if (error != TEXT_ERR_NO)
    {
        return error;
    }

    error = WordsArrayCtor(text);
    if (error != TEXT_ERR_NO)
    {
        return error;
    }

    return error;
}

TextErrors TextDtor(Text* text)
{
    assert((text != nullptr) && "Pointer to \'text\' is NULL!!!\n");

    TextErrors error = BufferDtor(text);

    free(text->words_array);
    text->words_array       = nullptr;
    text->words_array_size  = 0;

    return error;
}

TextErrors TextPrint(Text* text, const char* file_name)
{
    assert((text        != nullptr) && "Pointer to \'text\'      is NULL!!!\n");
    assert((file_name   != nullptr) && "Pointer to \'file_name\' is NULL!!!\n");

    FILE* outfile_ptr = nullptr;
    TextErrors error = OpenFile(file_name, &outfile_ptr, "w");
    if (error != TEXT_ERR_NO)
    {
        return error;
    }

    for (size_t i = 0; i < text->words_array_size; i++)
    {
        fprintf(outfile_ptr, "%.*s\n", text->words_array[i].size, text->words_array[i].begin);
    }

    return error;
}

void TextVerify(Text* text, TextErrors error)
{
    switch(error)
    {
        case TEXT_ERR_FILE_CLOSE:
            fprintf(stderr, "ERROR! Program can not close the FILE!\n");
            break;

        case TEXT_ERR_FILE_EMPTY:
            fprintf(stderr, "ERROR! FILE is EMPTY!\n");
            break;

        case TEXT_ERR_FILE_OPEN:
            fprintf(stderr, "ERROR! Program can not open the FILE!\n");
            break;

        case TEXT_ERR_FREAD:
            fprintf(stderr, "ERROR! fread is incorrect!\n");
            break;

        case TEXT_ERR_MEM_ALLOC:
            fprintf(stderr, "ERROR! Program can not allocate memory!\n");
            break;

        case TEXT_ERR_NO:
            break;

        default:
            fprintf(stderr, "Unknown error = %d", error);
            break;
    }
}