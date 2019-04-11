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
#include "settings.h"
#include "html2text.h"

char *home_dir;

void read_config_line (const char *line);

void find_titles_in_dir (const char *dir_name)
{
    DIR *dir;
    struct dirent *de;

    dir = opendir(dir_name);

    if (dir == NULL)
    {
        printf("cannot open directory %s.\n", dir_name);
        return;
    }

    printf("finding titles in %s\n", dir_name);

    while ((de = readdir(dir)))
    {
        unsigned trash;
        // XXX find out what the max file path length is
        char full_path[1000];
        strncpy(full_path, dir_name, strlen(dir_name));
        strncpy(full_path + strlen(dir_name), de->d_name, strlen(de->d_name));
        full_path[strlen(dir_name) + strlen(de->d_name)] = '\0';

        load_book(full_path, &trash);
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

int is_white (char c)
{
    return c == ' ' || c == '\n';
}

void read_config_line (const char *line)
{
    char *p = strstr(line, "home=");

    if (p != NULL)
    {
        while ((char)*p != '/')
        {
            p ++;
        }

        char *end = p + strlen(p) - 1;

        while (is_white((char)*end) && end > p)
        {
            end --;
        }

        end[1] = '\0';

//        int len = strlen(p + 5);
        home_dir = malloc(strlen(p) * sizeof(char));
        strcpy(home_dir, p);
        printf("home directory %s.\n", p);
    }
}

int main (int argc, char **argv)
{
    int opt;
    bool failure = false;

    // -v verbose
    while ((opt = getopt(argc, argv, "v")) != -1)
    {
        switch (opt)
        {
        case 'v':
            verbose = true;
            break;
        default:
            failure = true;
            break;
        }
    }

    if (failure)
    {
        printf("usage: ereader [-v] [filename]\n");
        exit(0);
    }

    read_config();

    // open requested book (cmd line arg)
    if (optind < argc)
    {
        char *book = argv[argc - 1];
        unsigned book_id;
        int result = load_book(book, &book_id);

        printf("attempting to read %s\n", book);

        if (result != 0)
        {
            printf("could not open %s\n", book);
            return 1;
        }

    } else
    {
        find_titles_in_dir(home_dir);
        find_local_titles();
    }

    ui_start();

    books_cleanup();

    return 0;
}
