#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include "book.h"
#include "epub.h"
//#include "gtk_ui.h"
#include "term_ui.h"

void find_titles_in_dir (const char *dir_name)
{
    DIR *dir;
    struct dirent *de;

    dir = opendir(dir_name);

    if (dir == NULL)
    {
        return;
    }

    while (de = readdir(dir))
    {
        unsigned tmp;
        int ret = load_book(de->d_name, &tmp);
    }

    closedir(dir);
}

void find_local_titles ()
{
    find_titles_in_dir("./");
}

int main (int argc, char **argv)
{
    // open requested book (cmd line arg)
    if (argc > 1)
    {
        char *book = argv[1];
        printf("attempting to read %s\n", book);
        unsigned book_id;
        int result = load_book(book, &book_id);

        if (result != 0)
        {
            printf("could not open %s\n", book);
            return 1;
        }

    } else
    {
        find_local_titles();
    }

    ui_start();

    return 0;
}
