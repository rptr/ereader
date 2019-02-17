#include "book.h"
#include "epub.h"

int load_book (char *filename, unsigned *id)
{
    if (file_exists(filename) != 0)
    {
        return 1;
    }

    book_type type = get_file_type(filename);
    ebook book;

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

