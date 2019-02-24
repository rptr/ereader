#include "book.h"
#include "epub.h"

#include <string.h>
#include <stdlib.h>
#include <zip.h>

/*
 * this is quite hacky
 * get the root file (*.opf, from the container.xml file)
 * returns 0 upon success.
 */
int get_rootfile (char *filename)
{
    return 0;
}

/*
 * get a list of files (from the *.opf file / root file)
 * returns 0 uon success.
 */
int get_file_list (char **filenames)
{
    return 0;
}

int load_epub (char *filename, bookid book)
{
    int error;
    int result;
    char *opf_filename = NULL;
    // XXX magic number -- what's the MAX_PATH thing?
    char *directory = NULL;

    zip_t *fd = zip_open(filename, ZIP_RDONLY, &error);

    if (fd == NULL)
    {
        printf("could not open epub: %s\n", filename);
        return 1;

    } else 
    {
        result = find_rootfile(fd, &opf_filename);

        if (opf_filename == NULL || result != 0)
        {
            printf("can't find .opf file\n");
            return 2;
        }

        zip_file_t *opf_file;
        opf_file = zip_fopen(fd, opf_filename, ZIP_FL_UNCHANGED);

        if (opf_file == NULL)
        {
            printf("can't open .opf file %s\n", opf_filename);
            return 3;
        }

        // get the directory in which the .opf and (presumably) the other files sit 
        char *parts = strtok(opf_filename, "/");
        char *next;
        int i = 0;

        while (parts != NULL)
        {
            next = strtok(NULL, "/");

            if (next == NULL) break;

            if (directory == NULL)
            {
                directory = malloc(200);
            }

            strcpy(directory + i, parts);
            i += strlen(parts) + 1;
            directory[i - 1] = '/';
            directory[i] = '\0';

            parts = next;
        }

        printf("epub: looking for files in %s\n", directory);

        // get the actual html page file names from the content.opf file
        // 1. find position of ".html"
        // 2. search back until "\"" is found
        // 3. try to load xxxx.html
        char *pos;
        int offset = 0;
        int size;
        char buf[1024];

        while ((size = zip_fread(opf_file, buf + offset, 1024 - offset)) > 0)
        {
            while (offset < size)
            {
                pos = strstr(buf + offset, ".html");

                if (pos == NULL)
                {
                    break;
                }

                offset = pos - buf;

                int start = 0, len;

                for (; offset >= 0; offset --)
                {
                    if (buf[offset] == '"')
                    {
                        start = offset + 1;
                        // + 4 to account for the fact that we start at the '.'
                        // of ".html"
                        len = pos - buf - start + 5;

                        char *file = malloc(len * sizeof(char));;
                        strncpy(file, buf + start, len);
                        file[len] = '\0';

                        int success = load_file(fd, book, file, directory);
                        offset = pos - buf + len;
                        free(file);

                        if (success != 0)
                        {
                            printf("epub: could not load file %.*s.\n", len, buf + start);
                            return 6;
                        }

                        break;
                    }

                    if (offset == 0)
                    {
                        offset = size;
                        break;
                    }
                }
            }

            offset = 0;

            // we save the last little bit of the previous buffer when reading
            // a new chunk (in case there's a file/link in the last bit)
//            offset = 1024 - offset;
//            strncpy(buf, buf, offset);
        }

        zip_fclose(opf_file);
        zip_close(fd);    
    }

    // TEMP
    set_title(book, filename);

    // TODO remove multiple returns so that directory is always freed
    if (directory != NULL)
    {
        free(directory);
    }

    printf("EPUB: successfully loaded file %s\n", filename);
    return 0;
}

int find_rootfile (zip_t *zip, char **rootfile)
{
    zip_file_t *root_file = zip_fopen(zip, "META-INF/container.xml", ZIP_FL_UNCHANGED);

    if (root_file == NULL)
    {
        printf("can't load epub: no container.xml\n");
        return 1;
    }

    char buf[1024];
    int size;

    // find the .opf file which lists all the other content
    while ((size = zip_fread(root_file, buf, 1024)) > 0)
    {
        char *pos;
        pos = strstr(buf, "rootfile full-path");

        if (pos == NULL)
        {
            // no rootfile path
            printf("bad container.xml\n");
            return 4;
        }

        int index = pos - buf;
        int path_len = 0, path_start = 0;

        for (; index < size; index ++)
        {
            if (buf[index] == '"' && path_start == 0)
                path_start = index + 1;

            else if (buf[index] == '"')
            {
                path_len = index - path_start;
                *rootfile = malloc((path_len + 1) * sizeof(char));
                strncpy(*rootfile, buf + path_start, path_len);
                (*rootfile)[path_len] = '\0';
                break;
            }
        }

        if (path_len == 0)
        {
            printf("bad container.xml\n");
            return 5;
        }
    }

    zip_fclose(root_file);
    return 0;
}

int load_file (zip_t *zip, bookid book, char *filename, char *directory)
{
    char *full_path = filename;

    if (directory != NULL)
    {
        int len = strlen(filename) + strlen(directory) + 1;
        full_path = malloc(len);
        strcpy(full_path, directory);
        strcat(full_path, filename);
        full_path[len - 1] = '\0';
    }

    zip_file_t *file = zip_fopen(zip, full_path, ZIP_FL_UNCHANGED);

    if (file == NULL)
    {
        printf("epub: can't load file %s\n", full_path);

    } else
    {
        char *section = NULL;
        char buf[2048];
        int size, total_size = 0;
    
        while ((size = zip_fread(file, buf, 2048)) > 0)
        {
            char *temp = malloc((total_size + size + 1) * sizeof(char));
           
            if (section != NULL)
            {
                strncpy(temp, section, total_size);
                free(section);
            } 
    
            section = temp;
            strncpy(section + total_size, buf, size);
            total_size += size;
        }
    
        if (section == NULL)
        {
            printf("epub.c::load_file(): no text loaded\n");

        } else
        {
            char *plain = NULL;
        
            section[total_size] = '\0';
            html_to_plain(section, &plain);
            add_section(book, plain);
        }

        zip_fclose(file);
    }
    
    if (directory != NULL)
    {
        free(full_path);
    }

    return 0;
}

int html_to_plain (const char *html, char **plain)
{
    int len = strlen(html);
    char *clean = malloc(len * sizeof(char));
    bool rmv = false;
    int j = 0;

    // remove < > tags
    for (int i = 0; i < len; i ++)
    {
        if (html[i] == '<')
        {
            rmv = true;
        }

        if (!rmv)
        {
            clean[j] = html[i];
            j ++;
        }

        if (html[i] == '>')
        {
            rmv = false;
        }
    }

    *plain = malloc(len * sizeof(char));
    strncpy(*plain, clean, len);

    return 0;
}


