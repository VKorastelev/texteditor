#include "definitions.h"
#include "visual.h"


#include <string.h>


static char const *bottom_menu_key[] = {
                            "F1", "Help",
                            "F2", "Save",
                            "F3", "SaveAs",
                            "F4", "",
                            "F5", "CommMode",
                            "F6", "Goto",
                            "F7", "Search",
                            "F8", "EditMode",
                            "F9", "",
                            "10", "Quit"};


static int render_wedit(struct textedit_session const *const t_s);


static void top_menu(struct textedit_session const *t_s);


static void bottom_menu(struct textedit_session const *t_s);


void sizes_update(
        struct textedit_session *const t_s,
        int lines,
        int cols)
{
    t_s->height = lines;
    t_s->width = cols;
    t_s->wedit_h = t_s->height - 4;

    if (t_s->wedit_h < 0)
    {
        t_s->wedit_h = 0;
    }

    t_s->wedit_w = t_s->width;
    if (t_s->wedit_w < 0)
    {
        t_s->wedit_w = 0;
    }
}


int paint_resize(struct textedit_session *const t_s)
{
    int ret = 0;

    endwin();
    refresh();

    clear();
    wclear(t_s->wedit);

    if (t_s->height < 5 || t_s->width < 30)
    {
        ret = -1;
        goto finally;
    }

    wresize(t_s->wedit, t_s->wedit_h, t_s->wedit_w);

 finally:

    return ret;
}


void rendering(struct textedit_session *const t_s)
{
    render_wedit(t_s);

    wmove(t_s->wedit, t_s->y, t_s->x);

    top_menu(t_s);
    
    mvhline(1, 0, 0, t_s->width);
    mvhline(t_s->height - 2, 0, 0, t_s->width);

    bottom_menu(t_s);

    refresh();
    
    wrefresh(t_s->wedit);
}


int modal_win(
        int nlines,
        int ncols,
        int begin_y,
        int begin_x,
        char *header,
        int cursor_state,
        int box)
{
    int ret = 0;
    int ch = 0;
    int curs_old = 0;

    if (nlines < 3 || ncols < 3 || begin_y < 0 || begin_x < 0)
    {
        ret = -1;
        goto finally;
    }

    WINDOW *wb = newwin(nlines, ncols, begin_y, begin_x);
    
    if (box)
    {
        box(wb, 0, 0);
    }

    if (cursor_state >= 0 && cursor_state < 3)
    {
        curs_old = curs_set(cursor_state);
    }

    if (NULL != header 
            && 0 != strlen(header) 
            && strlen(header) < ncols)
    {
        wattron(wb , A_REVERSE | A_BOLD);

        mvwprintw(
                wb,
                0,
                (ncols - strlen(header)) / 2,
                "%s",
                header);

        wattroff(wb, A_REVERSE | A_BOLD);
    }

    mvwprintw(wb, nlines - 1, 1, "<ESC to exit>");

    wmove(wb, 1, 1);

    wrefresh(wb);

    while (1)
    {
        ch = wgetch(wb);

        if (KEY_RESIZE == ch)
        {
            break;
        }
        
        if (27 == ch)
        {
            nodelay(wb, true);
            ch = wgetch(wb);
            nodelay(wb, false);
            if (-1 == ch)
            {
                break;
            }
        }
    }

    if (ERR !=curs_old)
    {
        curs_set(curs_old);
    }

    delwin(wb);

 finally:

    return ret;
}


static int render_wedit(struct textedit_session const *const t_s)
{
    int ret = 0;

    size_t str_len = 0;

    wclear(t_s->wedit);

    if (t_s->wedit_h < 1)
    {
        ret = -1;
        goto finally;
    }

    for (int i = 0; i < t_s->wedit_h; i++)
    {
        if (NULL != t_s->textline[i + t_s->scroll_pos]
                && (i + t_s->scroll_pos) < t_s->num_textlines)
        {
            str_len = t_s->textline[i + t_s->scroll_pos]->len;
            if (str_len > t_s->wedit_w)
            {
                str_len = t_s->wedit_w;
            }

            mvwaddnstr(
                    t_s->wedit,
                    i,
                    0,
                    t_s->textline[i + t_s->scroll_pos]->text,
                    str_len);
        }
        else
        {
            mvwaddch(t_s->wedit, i, 0, '~');
        }
    }
 finally:

    return ret;
}


static void top_menu(struct textedit_session const *const t_s)
{
    move(0, 0);
    clrtoeol();

    attron(A_REVERSE | A_BOLD);

    mvprintw(0, 0, "%s",
            (0 == t_s->edmode) ? "COMMAND MODE" : " EDIT MODE  ");

    attron(A_DIM);

    mvprintw(0, 15, "%cfile:%s",
            (0 == t_s->dont_save)? '*' : '*',
            t_s->file_name);

    mvprintw(0, t_s->width >> 1, "%d,%d/%d",
            t_s->scroll_pos + t_s->y + 1,
            t_s->x + 1,
            t_s->num_textlines);
        
    attroff(A_REVERSE | A_BOLD | A_DIM);
}


static void bottom_menu(struct textedit_session const *const t_s)
{
    int x = 0;
    int num_items = (sizeof(bottom_menu_key) / sizeof(*bottom_menu_key)) >> 1;
    float fstep = ((float)t_s->width) / num_items;

    for (int i = 0; i < num_items; i++)
    {
        x = (int)(fstep * i);
        
        attron(A_REVERSE | A_BOLD);

        mvaddnstr(t_s->height - 1, x, bottom_menu_key[i << 1], 2);

        attron(A_DIM);

        mvprintw(t_s->height - 1, x + 2, "%-*s", (int)(fstep * (i + 1)) - x - 2,
                bottom_menu_key[(i << 1) + 1]);

        attroff(A_REVERSE | A_BOLD | A_DIM);
    }
}
