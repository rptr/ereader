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

int load_epub (char *filename, ebook *book)
{
    int error;

    zip_t *fd = zip_open(filename, ZIP_RDONLY, &error);

    if (fd == NULL)
    {
        printf("could not open epub\n");

    } else 
    {
        zip_file_t *root_file = zip_fopen(fd, "META-INF/container.xml", ZIP_FL_UNCHANGED);
//        zip_int64_t container_id = zip_name_locate(fd, "container,xml");

        if (root_file == NULL)
        {
            printf("can't load epub: no container.xml\n");
            return 1;
        }

        char *opf_filename = NULL;
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
                    opf_filename = malloc(path_len * sizeof(char));
                    strncpy(opf_filename, buf + path_start, path_len);
                    opf_filename[path_len] = '\0';
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

        if (opf_filename == NULL)
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

        // get the actual html page file names
        char *pos;
        int offset = 0;

        while ((size = zip_fread(opf_file, buf + offset, 1024 - offset)) > 0)
        {
            while (offset < size)
            {
                pos = strstr(buf + offset, "item href");

                if (pos == NULL)
                {
                    break;
                }

                offset = pos - buf;

                int start = 0, len;

                for (; offset < size; offset ++)
                {
                    if (buf[offset] == '"' && start == 0)
                    {
                        start = offset + 1;

                    } else if (buf[offset] == '"')
                    {
                        len = offset - start;
                        char *file = malloc(len * sizeof(char));;
                        strncpy(file, buf + start, len);
                        file[len] = '\0';
                        int success = load_file(fd, book, file);
                        free(file);

                        if (success != 0)
                        {
                            return 6;
                        }

                        break;
                    }
                }
            }

            // we save the last little bit of the previous buffer when reading
            // a new chunk
            offset = 1024 - offset;
            strncpy(buf, buf, offset);
        }

        zip_fclose(opf_file);
        zip_close(fd);    
    }

    // TEMP
    set_title(book, filename);

    printf("EPUB: successfully loaded file %s\n", filename);
    return 0;
}

int load_file (zip_t *zip, ebook *book, char *filename)
{
    zip_file_t *file = zip_fopen(zip, filename, ZIP_FL_UNCHANGED);

    if (file == NULL)
    {
        printf("epub: can't load file %s\n", filename);
        return 1;
    }

    printf("epub: load file %s\n", filename);

    char *section = NULL;
    char buf[2048];
    int size, total_size = 0;

    while ((size = zip_fread(file, buf, 2048)) > 0)
    {
        char *temp = malloc((total_size + size) * sizeof(char));
       
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
        return 1;
    }

    char *plain = NULL;

    section[total_size] = '\0';
    html_to_plain(section, &plain);
    add_section(book, plain);

    zip_fclose(file);
    return 0;
}

int html_to_plain (const char *html, char **plain)
{
    int len = strlen(html);
    char *clean = malloc(len * sizeof(char));
    bool rmv = false;
    int j = 0;

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

    *plain = malloc(j * sizeof(char));
    strncpy(*plain, clean, j);
//    memcpy(plain, clean, j * sizeof(char));

    return 0;
}
















