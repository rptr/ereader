
CFLAGS += $(pkg-config --cflags gtk+-3.0)

ereader : main.o book.o epub.o gtk_ui.o
	gcc -o ereader main.o book.o epub.o gtk_ui.o

%.o : %.c
	gcc `pkg-config --cflags gtk+-3.0` -o $@ -c $< $(CLFAGS)

clean :
	rm ereader main.o book.o epub.o

