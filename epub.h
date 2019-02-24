#ifndef EPUB_H
#define EPUB_H

#include "book.h"
#include <zip.h>

struct ebook;

int load_epub (char *filename, bookid id);
int find_rootfile (zip_t *zip, char **rootfile);
int load_file (zip_t *zip, bookid book, char *filename, char *directory);
int html_to_plain (const char *html, char **plain);

#endif
