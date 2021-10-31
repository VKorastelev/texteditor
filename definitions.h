#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_


#include <ncurses.h>
#include <sys/ioctl.h>


#define MAX_TEXT_LINE 10000


typedef struct textline
{
    char *text;
    size_t len;
} textline;

typedef struct textedit_session
{
    int x;
    int y;
    int width;
    int height;
    WINDOW *wedit;
    int wedit_w;
    int wedit_h;
    int scroll_pos;
    int edmode;
    int dont_save;
    int win;
    int num_textlines;
    struct textline *textline[MAX_TEXT_LINE];
    char *file_name;
} textedit_session;

#endif
