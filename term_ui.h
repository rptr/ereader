#ifndef TERM_UI_H
#define TERM_UI_H

int ui_start ();
int input (char input);
int list_titles ();
int display_title (unsigned book_id);

void scroll_up ();
void scroll_down ();
void page_up ();
void page_down ();

int select_title ();

void quit();

#endif
