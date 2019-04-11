#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "html2text.h"
#include "settings.h"

int
remove_junk (const char *html)
{
    int     success = 0;
    int     fd[2];
    pid_t   child;
    FILE    *f;

    f = fopen("temp", "w");

    if (f == NULL)
    {
        dbgprintf("html_to_text: can't open temp\n");
        success = 2;
        
    }
    else
    {
        fprintf(f, "%s\r", html);
        fclose(f);
    }

    pipe(fd);

    if ((child = fork()) == -1 && success == 0)
    {
        dbgprintf("html_to_text::remove_junk error: can't fork\n");
        success = 1;
    }
    else if (success == 0)
    {
        if (child == 0)
        {
            int err;

            close(fd[0]);
            dup2(fd[1], 1);

            if ((err = execl("/bin/iconv", "/bin/iconv",
                             "-f", "utf8", "-t", "ascii//TRANSLIT",
                             "-o", "temp2",
                             "temp", NULL)) == -1)
            {
                dbgprintf("iconv error: %d %s\n", errno, strerror(errno));
            }

            exit(0);
        }
        else
        {
            // parent
        }
    }

    return success;
}

int
html_to_text (const char *html, char **ascii, unsigned *length)
{
    int     success = 0;
    int     fd[2];
    pid_t   child_pid;

    success = remove_junk(html);
    
    pipe(fd);

    if ((child_pid = fork()) == -1 && success == 0)
    {
        dbgprintf("html_to_text error: can't fork\n");
        success = 1;

    } else if (success == 0)
    {
        if (child_pid == 0)
        {
            int err;

            close(fd[0]);
            dup2(fd[1], 1);

            if ((err = execl("html2text-1.3.2a/html2text", "html2text", "-ascii", "-nobs", "temp2", NULL)) != -1)
            {
            } else
            {
                dbgprintf("html2text error: %d\n", errno);
                exit(0);
            }

        } else
        {
            close(fd[1]);

            int readn;
            int size = 1000000;
            int total_size = 0;
            char buf[size];

            while ((readn = read(fd[0], buf, size)))
            {
                char tmp[total_size];
                memcpy(tmp, *ascii, total_size);

                total_size += readn;

                if (*ascii)
                {
                    free(*ascii);
                }

                *ascii = malloc(total_size + 1);
                memcpy(*ascii, tmp, total_size - readn);
                memcpy(*ascii + total_size - readn, buf, readn);
                (*ascii)[total_size] = '\0';
            }

            *length = total_size;

            char fn[100];
            sprintf(fn, "C%d", total_size);
            FILE *f = fopen(fn, "w");

            if (f)
            {
                fwrite(*ascii, 1, total_size, f);
            }

            fclose(f);

            // better way to do this? TODO
//            while ((f = fopen("ascii", "r")) == NULL && tries < 100)
//            {
//                sleep(0.1);
//                tries ++;
//            }
//
//            if (tries == 100)
//            {
//                dbgprintf("html2text error: could not open ascii\n");
//
//            } else
//            {
//                long int len;
//
//                fseek(f, 0, SEEK_END);
//                len = ftell(f);
//                rewind(f);
//                *ascii = malloc(len + 1);
//                fread((char *) *ascii, 1, len, f);
//                (*ascii)[len] = '\0';
//                fclose(f);
//
//                printf("html2text len %ld %s\n", len, *ascii);
////                system("rm ascii");
//            }
        }
    }


    return success;
}



