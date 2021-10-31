#ifndef _TEXTLINES_H_
#define _TEXTLINES_H_


int loading_textlines(
        struct textedit_session *const t_s,
        FILE *const ptf);

void free_textlines(struct textedit_session *const t_s);


#endif
