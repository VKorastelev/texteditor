#include "definitions.h"
#include "textlines.h"
#include "keyboard.h"
#include "visual.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <locale.h>


static int file_access(char const *pathname);

static int init_textedit_session(struct textedit_session *t_s);

static int start_textedit_session(struct textedit_session *const t_s);

static void finally_textedit_session(struct textedit_session *const t_s);


int main(int argc, char *argv[])
{
    int ret = 0;
    FILE *ptf = NULL;
    int sv_errno = 0;
 
    struct textedit_session t_s = {0};

    //setlocale(LC_ALL, "ru_RU.utf8");
    setlocale(LC_ALL,"");

    if (2 != argc)
    {
        fprintf(stderr, "Usage: %s <text file pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    ret = file_access(argv[1]);

    errno = 0;
    switch (ret)
    {
        case 0:
        {
            ptf = fopen(argv[1], "r+");
            if (NULL == ptf)
            {
                sv_errno = errno;
                perror("Error opening the file");
                exit(sv_errno);
            }
            puts("The file is open");
            break;
        }
        case EACCES:
        {
            puts("Check file access rights!");
            exit(EXIT_FAILURE);
            break;
        }
        case ENOENT:
        {
            ptf = fopen(argv[1], "w+");
            if (NULL == ptf)
            {
                sv_errno = errno;
                perror("Error creating the file");
                exit(sv_errno);
            }

            puts("A new file has been created");
            break;
        }
        default:
        {
            puts("Some errors!");
            exit(EXIT_FAILURE);
        }
    }

    if (-1 == (ret = loading_textlines(&t_s, ptf)))
    {
        fprintf(stderr, "Error reading a text file!");
        goto finally;
    }

    t_s.file_name = argv[1];
    
    if (-1 == (ret = start_textedit_session(&t_s)))
    {
        fprintf(stderr, "Error in start textedit session!");
        goto finally;
    }

    keyboard_handler(&t_s);

 finally:

    finally_textedit_session(&t_s);

    fclose(ptf);

    return ret;
}


static int file_access(char const *const pathname)
{
    int ret = 0;
    errno = 0;

    ret = access(pathname, F_OK | R_OK);// | W_OK);
    if (0 != errno)
    {
        ret = errno;
        perror("Check the file rights!");
    }

    return ret;
}


int init_textedit_session(struct textedit_session *const t_s)
{
    int ret = 0;

    t_s->x = 0;
    t_s->y = 0;
    t_s->height = LINES;
    t_s->width = COLS;
    t_s->wedit_h = t_s->height - 4;
    t_s->wedit_w = t_s->width;
    t_s->scroll_pos = 0;
    t_s->edmode = 0;         // Command mode
    t_s->dont_save = 0;

    t_s->wedit = newwin(t_s->wedit_h, t_s->wedit_w, 2, 0);

    if (NULL == t_s->wedit)
    {
        ret = -1;
    }

    return ret;
}


int start_textedit_session(struct textedit_session *const t_s)
{
    int ret = 0;

    initscr();
    noecho();
    cbreak();

    ret = init_textedit_session(t_s);

    paint_resize(t_s);

    rendering(t_s);

    keypad(stdscr, TRUE);

    return ret;
}


void finally_textedit_session(struct textedit_session *const t_s)
{
    free_textlines(t_s);
    clear();
    delwin(t_s->wedit);
    endwin();
}
