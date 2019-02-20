#ifndef EPUB_H
#define EPUB_H

#include "book.h"
#include <zip.h>

struct ebook;

int load_epub (char *filename, bookid id);
int load_file (zip_t *zip, bookid book, char *filename);
int html_to_plain (const char *html, char **plain);

#endif
