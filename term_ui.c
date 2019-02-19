#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "term_ui.h"
#include "book.h"

int offset = 0;

int ui_start ()
{
    bool run = true;

    list_titles();

    while (run)
    {
        char i = getchar();

        input(i);
    }

    return 0;
}

int input (char i)
{
    printf("in: %d\n", i);
    return 0;
}

int list_titles ()
{
    ebook *books;
    int num_books;

    get_all_books(&books, &num_books);

    printf("LIBRARY | %d titles  | \n", num_books);

    for (int i = 0; i < num_books; i ++)
    {
        const char *title = book_title(&books[i]);
        printf("%d: %s\n", i + 1, title);
    }

    return 0;
}

int display_title (unsigned book_id)
{
    return 0;
}

void scroll_up ()
{

}

void scroll_down ()
{

}

void page_up ()
{

}

void page_down ()
{

}

int select_title ()
{
    return 0;
}

void quit ()
{

}

