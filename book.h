#ifndef BOOK_H
#define BOOK_H

#include <stdbool.h>

/*
 * Internal e-book format.
 */

typedef enum
{
    EPUB = 0,
    MOBI = 1,
    INVALID,

} book_type;

typedef struct
{
    unsigned    id;
    char        *title;
    char        *author;
    char        *body;

} ebook;

typedef unsigned bookid;

extern ebook *books;
extern int num_books;
extern int max_books; 

bool file_exists (char *filename);
const char *get_file_extension (const char *filename);
book_type get_file_type (char *filename);

/*
 *
 */
ebook *new_book ();

void remove_book (bookid id);

/*
 * id is set to the id of the loaded book
 */
int load_book (char *filename, unsigned *id);

/*
 * Initialise a new ebook and give its id.
 * Returns 0 on success.
 */
int init_ebook (unsigned *id);

/*
 * Gives a pointer to an ebook.
 * Returns 0 on success.
 */
int get_ebook (unsigned id, ebook *book);

int add_section (bookid book, char *text);

int set_title (bookid id, const char *text);

const char *get_title (bookid id);

unsigned get_num_books ();

/*
 * Returns title of book.
 */
const char *book_title (bookid id);

#endif
