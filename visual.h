#ifndef _VISUAL_H_
#define _VISUAL_H_


void sizes_update(
        struct textedit_session *t_s,
        int lines,
        int cols);

int paint_resize(struct textedit_session *t_s);

void rendering(struct textedit_session *const t_s);

int modal_win(
        int nlines,
        int ncols,
        int begin_y,
        int begin_x,
        char *header,
        int cursor_state,
        int box);


#endif
