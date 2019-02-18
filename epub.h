#ifndef EPUB_H
#define EPUB_H

#include <zip.h>

struct ebook;

int load_epub (char *filename, ebook *book);
int load_file (zip_t *zip, ebook *book, char *filename);
int html_to_plain (const char *html, char **plain);

#endif
