#include "definitions.h"
#include "visual.h"
#include "keyboard.h"


static void cursor_position_adjust(struct textedit_session *const t_s);


int keyboard_handler(struct textedit_session *const t_s)
{
    int ret = 0;
    int ch = 0;

    while (1)
    {
        ch = getch();

        switch (ch)
        {
            case KEY_RESIZE:
            {
                sizes_update(t_s, LINES, COLS);
                paint_resize(t_s);

                break;
            }
            case KEY_F(1):
            {
                modal_win(
                        20,
                        40,
                        (t_s->height - 20) / 2,
                        (t_s->width - 40) / 2,
                        "Help",
                        0,
                        1);
                break;
            }
            case KEY_F(5):
            {
                if (0 != t_s->edmode)
                {
                    t_s->edmode = 0;
                }
                break;
            }
            case KEY_F(8):
            {
                if (0 == t_s->edmode)
                {
                    t_s->edmode = 1;
                }
                break;
            }
            case KEY_F(7):
            {
                modal_win(
                        3,
                        30,
                        (t_s->height - 3) / 2,
                        (t_s->width - 30) / 2,
                        "Input search string!",
                        1,
                        1);
                break;
            }
            case KEY_F(10):
            {
                goto finally;
                break;
            }
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
            {
                arrow_key(t_s, ch);
                break;
            }
            default:
            {
            }
        }
        rendering(t_s);
    }

 finally:

    return ret;
}


void arrow_key(
        struct textedit_session *const t_s,
        int ch)
{
    switch (ch)
    {
        case KEY_UP:
        {
            if (t_s->y > 0)
            {
                t_s->y--;
            }
            else if (t_s->scroll_pos > 0)
            {
                t_s->scroll_pos--;
            }
            else
            {
                beep();
            }
            break;
        }
        case KEY_DOWN:
        {
            if ((t_s->y + t_s->scroll_pos + 1) < t_s->num_textlines)
            {
                if (t_s->y < (t_s->wedit_h - 1))
                {
                    t_s->y++;
                }
                else
                {
                    t_s->scroll_pos++;
                }
            }
            else
            {
                beep();
            }
            break;
        }
        case KEY_LEFT:
        {
            if (t_s->x > 0)
            {
                t_s->x--;
            }
            else
            {
                beep();
            }
            break;
        }
        case KEY_RIGHT:
        {
            if (NULL == t_s->textline[t_s->y + t_s->scroll_pos])
            {
                beep();
                t_s->x = 0;
                goto finally;
            }

            if (t_s->x + 1 < t_s->textline[t_s->y + t_s->scroll_pos]->len
                    && t_s->x < t_s->wedit_w - 1)
            {
                t_s->x++;
            }
            else
            {
                beep();
            }
            break;
        }
        default:
        {
        }
    }

    cursor_position_adjust(t_s);

 finally:

    return;
}


void cursor_position_adjust(struct textedit_session *const t_s)
{
    if (NULL == t_s->textline[t_s->y + t_s->scroll_pos])
    {
        t_s->x = 0;
        goto finally;
    }

    if (t_s->textline[t_s->y + t_s->scroll_pos]->len == 0)
    {
        t_s->x = 0;
    }
    else if (t_s->x + 1 > t_s->textline[t_s->y + t_s->scroll_pos]->len)
    {
        t_s->x = t_s->textline[t_s->y + t_s->scroll_pos]->len - 1;
    }

 finally:

    return;
}
