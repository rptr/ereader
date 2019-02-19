
CFLAGS=-Wall -g -ggdb `pkg-config --cflags gtk+-3.0 libzip`
LIBS=`pkg-config --libs gtk+-3.0 libzip`

ereader : main.o book.o epub.o gtk_ui.o term_ui.o
	gcc -o ereader main.o book.o epub.o gtk_ui.o term_ui.o $(LIBS) $(CFLAGS)

%.o : %.c
	gcc -o $@ -c $< $(LIBS) $(CFLAGS)

clean :
	rm ereader main.o book.o epub.o gtk_ui.o term_ui.o

