#include "book.h"
#include "epub.h"
#include "settings.h"

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
    char *ext = malloc(5 * sizeof(char));

    for (int i = 0; i < len; i ++)
    {
        for (int j = 1; j < 4; j ++)
        {
            ext[j - 1] = ext[j];
        }

        ext[3] = filename[i];
    }

    ext[4] = '\0';

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
        // XXX TODO you are not fuckng copying over the old books
        free(books);
        books = malloc(max_books * sizeof(ebook));
    }
}

ebook *new_book ()
{
    ebook *book;

    resize_library();
    book = &books[num_books++];
    book->id = num_books - 1;
    book->body = NULL;
    book->title = NULL;

    return book;
}

// XXX only works for books at the end of the array
void remove_book (bookid id)
{
    if (id == num_books - 1)
    {
        num_books --;
    }
}

int load_book (char *filename, unsigned *id)
{
    if (file_exists(filename) != 0)
    {
        printf("no such file %s\n", filename);
        return 1;
    }

    book_type type = get_file_type(filename);

    if (type == INVALID)
    {
        return 2;
    }

    ebook *book;
    bookid bid;
    int result = 0;

    switch (type)
    {
        case EPUB:
            book = new_book();
            bid = book->id;
            result = load_epub(filename, bid);
            break;
        case MOBI:
//            load_mobi(filename, &book);
            break;
        default:
            // can't load format / not an ebook
            break;
    }

    if (result != 0)
    {
        remove_book(bid);
    }

    return 0;
}

int get_ebook (unsigned id, ebook *book)
{
    return 1;
}

const char *book_title (bookid id)
{
    return books[id].title;
}

int init_ebook (unsigned *id)
{
    id = 0;

    return 1;
}

int set_title (bookid id, const char *text)
{
    if (text == NULL)
    {
        printf("book.c::set_title(): NULL text\n");
        return 1;
    }

    dbgprintf("set_title: %d %s\n", id, text);
    books[id].title = malloc(strlen(text) * sizeof(char));
    strcpy(books[id].title, text);
    return 0;
}

const char *get_title (bookid id)
{
    if (id >= num_books)
    {
        return "INVALID TITLE";
    }

    return books[id].title;
}

/*
 * 
 */
int add_section (bookid book, char *text, unsigned length)
{
    if (text == NULL)
    {
        dbgprintf("book.c::add_section(): no text\n");
        return 1;
    }

    dbgprintf("add section book id: %d. len: %u\n", book, length);

    if (books[book].body != NULL)
    {
        char *new;
        int old_len = books[book].body_size;
        int new_len = old_len + length;

        new = malloc(new_len + 1);
        memcpy(new, books[book].body, old_len);
        memcpy(new + old_len, text, length);

        free(books[book].body);
        books[book].body = new;
        books[book].body_size = new_len;

    } else
    {
        books[book].body = malloc(length + 1);
        strcpy(books[book].body, text);
        books[book].body_size = length;
    }

    books[book].body[books[book].body_size] = '\0';

    printf("new len %d\n", books[book].body_size);

    return 0;
}

int get_page (bookid book, char **text, int offset, int length)
{
    int max_len;
    ebook *ebook;

    if (book >= num_books)
    {
        printf("book.c::get_page(): invalid book %d\n", book);
        return 2;
    }

    ebook = &books[book];

    if (ebook->body == NULL)
    {
        printf("book.c: requested book has no text.\n");
        return 1;
    }

    max_len = strlen(ebook->body);

    if (offset > max_len)
    {
        printf("book.c: requested page outside of book %d %d\n", max_len, offset);
        return 2;
    }

    if (offset + length > max_len)
    {
        length = max_len - offset;
    }

    *text = ebook->body + offset;
//    *text = malloc(length); 
//    strncpy(*text, ebook.body + offset, length);

    return 0;
}

const char *get_body (bookid id)
{
    if (id >= num_books)
    {
        printf("book.c::get_page(): invalid book %d\n", id);
        return 0;
    }

    return books[id].body;
}

unsigned get_num_books ()
{
    return num_books;
}

void books_cleanup ()
{
    for (int i = 0; i < num_books; i ++)
    {
        free(books[i].title);
//        free(books[i].author);
        free(books[i].body);
    }

    free(books);
}


