#include "book.h"
#include "epub.h"

#include <stdlib.h>

bool file_exists (char *filename)
{
    return 0;
}

book_type get_file_type (char *filename)
{
    return EPUB;
}

int load_book (char *filename, unsigned *id)
{
    if (file_exists(filename) != 0)
    {
        return 1;
    }

    book_type type = get_file_type(filename);
    ebook book;
    book.id = num_books++;

    switch (type)
    {
        case EPUB:
            load_epub(filename, &book);
            break;
        case MOBI:
//            load_mobi(filename, &book);
            break;
        default:
            // cannot load file type
            break;
    }

    return 0;
}

int get_ebook (unsigned id, ebook *book)
{
    return 1;
}

int init_ebook (unsigned *id)
{
    id = 0;

    return 1;
}

/*
 * 
 */
int add_section (ebook *book, char *text)
{
    if (text == NULL)
    {
        printf("book.c::add_section(): no text\n");
        return 1;
    }

    book->body = text;

    return 0;
}

