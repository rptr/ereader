#ifndef TERM_UI_H
#define TERM_UI_H

#include "book.h"

extern bookid current_book;
extern int selection;
extern int last_input;

typedef enum
{
    LIBRARY,
    BOOK

} State;

extern State state;

int ui_start ();

/*
 * Clear terminal and add border, title bar
 * title: text at top of screen
 */
void clear_screen (const char *title);

int input (char input);

/*
 * Display the user's library.
 */
int list_titles ();
int display_book ();
void display_book_text (const char *text);

void scroll_up ();
void scroll_down ();
void page_up ();
void page_down ();
void change_selection (int amount);

/*
 */
void select_title();

void quit();

#endif
