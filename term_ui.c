#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <string.h>
#include <locale.h>

#include "term_ui.h"
#include "book.h"
#include "settings.h"

int book_page = 0;
int selection = 0;
bookid current_book = 0;
int last_input = 0;
State state = LIBRARY;

int ui_start ()
{
    bool run = true;

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);

    while (run)
    {
        if (LIBRARY == state)
        {
            list_titles();
        }

        if (BOOK == state)
        {
            display_book();
        }

        char i = getchar();

        if (input(i) != 0)
        {
            run = false;
        }
    }

    endwin();

    return 0;
}

void clear_screen (const char *title_full)
{
    int w, h;

    getmaxyx(stdscr, h, w);

    clear();

    const char app[7] = "ereader";

    // TODO truncate length depending on width of terminal
    char *title = malloc(20 * sizeof(char));
    strncpy(title, title_full, 20);
    int title_len = 20;

    const char version[3] = "0.1";
    char *top = malloc(w * sizeof(char));
    // 178 = solid block
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
    last_input = i;

    switch (i)
    {
        // up arrow
        case 65:
        case 'k':
            scroll_up();
            break;
        // down arrow
        case 66:
        case 'j':
            scroll_down();
            break;
        // left arrow
        case 61:
            break;
        // right arrow
        case 63:
            break;

        // page up
        case 'J':
            page_up();
            break;
        // page down
        case 'K':
            page_down();
            break;

        // enter
        case 13:
            select_title();
            break;

        case 'q':
            return 1;
            break;

        default:
            break;
    }

    return 0;
}

int list_titles ()
{
    clear_screen("Library");

    int num_books = get_num_books();
    int w, h;

    getmaxyx(stdscr, h, w);

    for (int i = 0; i < num_books; i ++)
    {
        const char *title = book_title(i);

        if (NULL == title)
        {
            continue;
        }

        char selected = ' ';

        if (selection == i)
        {
            selected = 'X';
        }

        int max = w - 20;
        char *title_trunc = malloc(max * sizeof(char));
        strncpy(title_trunc, title, max);
        title_trunc[max - 1] = '\0';
        mvprintw(i + 1, 1, "%c %d: %s", selected, i + 1, title_trunc);
        free(title_trunc);
    }

    // show input in bottom left
    char str[10];
    sprintf(str, "%d", last_input);

    getmaxyx(stdscr, h, w);
    mvprintw(h - 1, 1, str); 

    refresh();

    return 0;
}

int display_book ()
{
    char *text = NULL;
    int length;
    int result;
    const char *title = get_title(current_book);
    clear_screen(title);
    int w;
    int h;
    int w2;
    int h2;
   
    getmaxyx(stdscr, h, w);
    w2 = w - 2 - padding_x * 2;
    h2 = h - 2 - padding_y * 2;
    length = w2 * h2;

    text = get_body(current_book) + book_page * 500;

    int x = 0;
    int y = 0;
    int len = h2 * w2;

    for (int i = 0; i < len; i ++)
    {
        char c = text[i];

        if (c == '\n')
        {
            y ++;
            x = 0;
            continue;
        }

        // ascii
        if ((unsigned char)c < 128)
        {
            mvaddch(1 + padding_y + y, 1 + padding_x + x, c);

        // unicode
        } else if ((unsigned char)c < 224)
        {
            mvprintw(1 + padding_y + y, 1 + padding_x + x, "%.*s", 2, text + i);
            i ++;

        } else if ((unsigned char)c < 240)
        {
            mvprintw(1 + padding_y + y, 1 + padding_x + x, "%.*s", 3, text + i);
            i += 2;

        } else
        {
            mvprintw(1 + padding_y + y, 1 + padding_x + x, "%.*s", 4, text + i);
            i += 3;
        }

        x ++;

        if (x == w2)
        {
            x = 0;
            y ++;
        }
    }

    refresh();

    return 0;
}

void scroll_up ()
{
    if (state == LIBRARY)
    {
        selection --;

        if (selection < 0)
        {
            selection = get_num_books() - 1;
        }
    }

    if (state == BOOK)
    {
        book_page --;

        if (book_page < 0)
        {
            book_page = 0;
        }
    }
}

void scroll_down ()
{
    if (state == LIBRARY)
    {
        selection ++;

        if (selection >= get_num_books())
        {
            selection = 0;
        }
    }

    if (state == BOOK)
    {
        book_page ++;
    }
}

void page_up ()
{
    if (state == BOOK)
    {
    }
}

void page_down ()
{
    if (state == BOOK)
    {
    }
}

void select_title ()
{
    if (LIBRARY == state)
    {
        current_book = selection;
        state = BOOK;
    }
}

void quit ()
{

}

