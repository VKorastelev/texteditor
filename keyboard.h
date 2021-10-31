#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_


int keyboard_handler(struct textedit_session *const t_s);

void arrow_key(
        struct textedit_session *const t_s,
        int ch);


#endif
