#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "term_ui.h"

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
    printf("1. asdads \n2. asdasda \n3. jzxijxz\n");
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

