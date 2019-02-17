#ifndef BOOK_H
#define BOOK_H

/*
 * Internal e-book format.
 */

typedef enum
{
    EPUB = 0,
    MOBI = 1,

} book_type;

typedef struct
{
    unsigned    id;
    unsigned    size;
    char        *title;
    char        *author;
    char        *body;

} ebook;

int file_exists (char *filename);
book_type get_file_type (char *filename);

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

#endif
