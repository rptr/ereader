#include "book.h"
#include "epub.h"

#include <stdlib.h>
#include <string.h>

ebook *books = NULL;
int max_books = 100;
int num_books = 0;

bool file_exists (char *filename)
{
    return 0;
}

const char *get_file_extension (const char *filename)
{
    int len = strlen(filename);
    char *ext = malloc(4 * sizeof(char));

    for (int i = 0; i < len; i ++)
    {
        for (int j = 1; j < 4; j ++)
        {
            ext[j - 1] = ext[j];
        }

        ext[3] = filename[i];
    }

    return ext;
}

// TODO: check actual file internals to see if it's an epub, not just file-ending
book_type get_file_type (char *filename)
{
    const char *ext = get_file_extension(filename);

    if (strcmp(ext, "epub") == 0)
    {
        return EPUB;
    }

    if (strcmp(ext, "mobi") == 0)
    {
        return MOBI;
    }

    return INVALID;
}

void resize_library ()
{
    if (books == NULL)
    {
        books = malloc(max_books * sizeof(ebook));

    } else if (num_books >= max_books)
    {
        max_books *= 2;
        free(books);
        books = malloc(max_books * sizeof(ebook));
    }
}

ebook *new_book ()
{
    ebook *book;

    resize_library();
    book = &books[num_books++];

    return book;
}

int load_book (char *filename, unsigned *id)
{
    if (file_exists(filename) != 0)
    {
        return 1;
    }

    book_type type = get_file_type(filename);

    if (type == INVALID)
    {
        return 2;
    }

    ebook *book = new_book();

    switch (type)
    {
        case EPUB:
            load_epub(filename, book);
            break;
        case MOBI:
//            load_mobi(filename, &book);
            break;
        default:
            // can't load format / not an ebook
            break;
    }

    return 0;
}

int get_ebook (unsigned id, ebook *book)
{
    return 1;
}

void get_all_books (ebook **books_r, int *num_books_r)
{
    *num_books_r = num_books;
    *books_r = books;
}

int init_ebook (unsigned *id)
{
    id = 0;

    return 1;
}

int set_title (ebook *book, const char *text)
{
    if (text == NULL)
    {
        return 1;
    }

    book->title = text;
    return 0;
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

const char *book_title (ebook *book)
{
    return book->title;
}

