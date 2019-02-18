
#include "book.h"
#include "epub.h"
#include "gtk_ui.h"

int main (int argc, char **argv)
{
    if (argc > 1)
    {
        char *book = argv[1];
        printf("attempting to read %s\n", book);
        int book_id;
        int result = load_book(book, &book_id);

    } else
    {
        printf("please provide a filename\n");
        return 1;
    }

    ui_init_window();

    return 0;
}
