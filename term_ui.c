#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <string.h>

#include "term_ui.h"
#include "book.h"

int offset = 0;
int selection = 0;
State state = LIBRARY;

int ui_start ()
{
    bool run = true;

    initscr();
    noecho();

    list_titles();

    while (run)
    {
        char i = getchar();

        input(i);
    }

    endwin();

    return 0;
}

void clear_screen ()
{
    int w, h;

    getmaxyx(stdscr, h, w);

    clear();

    const char app[7] = "ereader";
    int title_len = 20;
    const char title[20] = "some book";
    const char version[3] = "0.1";
    char *top = malloc(w * sizeof(char));
    char border = (char)178;

    // top bar
    for (int x = 0; x < w; x ++)
    {
        top[x] = border;
    }

    strncpy(top + 1, app, 7);
    strncpy(top + w / 2 - title_len / 2, title, title_len);
    strncpy(top + w - 5, version, 3);

    for (int x = 0; x < w; x ++)
    {
        mvaddch(0, x, top[x]);
        mvaddch(h - 1, x, border);
    }

    free(top);

    for (int y = 0; y < h; y ++)
    {
        mvaddch(y, 0, border);
        mvaddch(y, w - 1, border);
    }

    move(1, 1);

    refresh();
}

int input (char i)
{
    printf("in: %d\n", i);
    return 0;
}

int list_titles ()
{
    clear_screen();

    int num_books = get_num_books();

    for (int i = 0; i < num_books; i ++)
    {
        const char *title = book_title(i);
        char selected = ' ';

        if (selection == i)
        {
            selected = 'X';
        }

        mvprintw(1, 1 + i, "%c %d: %s\n", selected, i + 1, title);
    }

    refresh();

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

