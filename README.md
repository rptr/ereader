# ereader

My little e-book reader

Formats: EPUB.

Was annoyed with sluggishness of most e-book readers available, this works 
but... it's sluggish. Seems to be the way I process the HTML is the problem,
I switched from my own code to html2text + iconv + writing and reading to
and from files while converting. Before I simply stripped all <> tags.

TODO
* improve html -> plaintext conversion
* extract the title of the books (EPUB) for display in the library list instead of filename
* select text while reading
    * save snippets (where to store them?)
    * add comments
* dictionary lookup of selected word
* MOBI support
* non-terminal user interface

![Alt text](/screen_1.png?raw=true "ereader running in GNOME Terminal")

dependencies:

`libzip libncursesw5`

to compile:

```
cd html2text-1.3.2a
./configure
./make
cd ..
make
```

if you have any issues with compiling html2text, please refer to the INSTALL file in the html2text directory
