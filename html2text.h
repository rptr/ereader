#ifndef HTML2TEXT_H
#define HTML2TEXT_H

/*
 * Takes a string of html and a pointer to a string, 
 * puts formatted plaintext in *ascii.
 * Returns 0 on success.
 */
int html_to_text (const char *html, char **ascii, unsigned *length);

#endif
