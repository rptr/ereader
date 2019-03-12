#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "html2text.h"
#include "settings.h"

int
html_to_text (const char *html, char **ascii)
{
    int     success;
    pid_t   child_pid;

    success = 0;

    system("rm temp");
    system("rm ascii");

    FILE *f = fopen("temp", "w");

    if (f == NULL)
    {
        dbgprintf("html_to_text: can't open temp\n");
        success = 2;
        
    } else
    {
        fprintf(f, "%s\r", html);
        fclose(f);
    }

    if ((child_pid = fork()) == -1 && success == 0)
    {
        dbgprintf("html_to_text error: can't fork\n");
        success = 1;

    } else if (success == 0)
    {
        if (child_pid == 0)
        {
            int err;

            if ((err = execl("html2text-1.3.2a/html2text", "html2text", "-o", "ascii", "temp", NULL)) != -1)
            {
                exit(0);

            } else
            {
                dbgprintf("html2text error: %d\n", errno);
            }

        } else
        {
            FILE *f;
            int tries = 0;

            // better way to do this?
            while ((f = fopen("ascii", "r")) == NULL && tries < 20)
            {
                sleep(1);
                tries ++;
            }

            if (tries == 20)
            {
                dbgprintf("html2text error: could not open ascii\n");

            } else
            {
                long int len;

                fseek(f, 0, SEEK_END);
                len = ftell(f);
                rewind(f);
                *ascii = malloc(len + 1);
                fread((char *) *ascii, 1, len, f);
                (*ascii)[len] = '\0';

                fclose(f);
            }

            system("rm temp");
            system("rm ascii");
        }
    }

    return success;
}



