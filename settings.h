#ifndef SETTINGS_H
#define SETTINGS_H

extern int verbose;

#define dbgprintf(...) \
    do { if (verbose) fprintf(stderr, __VA_ARGS__); } while (0)

extern int line_spacing;
extern int padding_x;
extern int padding_y;

int load_settings ();

#endif
