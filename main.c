#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>

#include "book.h"
#include "epub.h"
//#include "gtk_ui.h"
#include "term_ui.h"

char *home_dir;

void read_config_line (const char *line);

void find_titles_in_dir (const char *dir_name)
{
    DIR *dir;
    struct dirent *de;

    dir = opendir(dir_name);

    if (dir == NULL)
    {
        printf("cannot open directory %s\n", dir_name);
        return;
    }

    printf("finding titles in %s\n", dir_name);

    while ((de = readdir(dir)))
    {
        unsigned tmp;
        load_book(de->d_name, &tmp);
    }

    closedir(dir);
}

void find_local_titles ()
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        return;
    }

    int len = strlen(cwd);
    cwd[len] = '/';
    cwd[len + 1] = '\0';

    find_titles_in_dir(cwd);
}

/*
 * Checks for the file ~/.ereader, for user configuration
 */
void read_config ()
{
    FILE *fd;
    char line[200];
    struct passwd *pwd = getpwuid(geteuid());
    char *path;
    const char *filename = "/.ereader";

    if (pwd)
    {
        path = malloc(strlen(pwd->pw_dir) + strlen(filename));
        strcpy(path, pwd->pw_dir);
        strcat(path, filename);
    }

    fd = fopen(path, "r");

    if (fd != NULL)
    {
        fgets(line, 200, fd);

        if (line != NULL)
        {
            read_config_line(line);            
        }

        fclose(fd);

    } else
    {
        printf("could not open configuration file ~/.ereader\n");
    }

    free(path);
}

void read_config_line (const char *line)
{
    char *p = strstr(line, "home=");

    if (p != NULL)
    {
        int len = strlen(p + 5);
        home_dir = malloc(len * sizeof(char));
        strcpy(home_dir, p + 5);
        printf("home directory %s\n", home_dir);
    }
}

int main (int argc, char **argv)
{
    read_config();

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
