#ifndef BOOK_H
#define BOOK_H

/*
 * Internal e-book format.
 */

typedef struct
{
    unsigned    id;
    unsigned    size;
    char        *title;
    char        *author;
    char        *body;

} ebook;

/*
 * Initialise a new ebook and give its id.
 * Returns 0 on success.
 */
int init_ebook (unsigned &id);

/*
 * Gives a pointer to an ebook.
 * Returns 0 on success.
 */
int get_ebook (unsigned id, ebook *book);

#endif
