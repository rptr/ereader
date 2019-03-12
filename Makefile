
CFLAGS=-Wall -g -ggdb `pkg-config --cflags libzip`
LIBS=`pkg-config --libs libzip ncursesw`

ereader : main.o book.o epub.o gtk_ui.o term_ui.o settings.o html2text.o
	gcc -o ereader main.o book.o epub.o gtk_ui.o term_ui.o settings.o html2text.o $(LIBS) $(CFLAGS)

%.o : %.c
	gcc -o $@ -c $< $(LIBS) $(CFLAGS)

clean :
	rm ereader main.o book.o epub.o gtk_ui.o term_ui.o settings.o html2text.o

