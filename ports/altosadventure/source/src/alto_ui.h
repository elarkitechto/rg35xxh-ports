#ifndef ALTO_UI_H
#define ALTO_UI_H
struct alto_ui_input {int confirm,back,pause,dx,dy,tab;};
void alto_ui_init(void *(*resolve)(const char *));
int alto_ui_tick(int frame,int state,void *state_manager,struct alto_ui_input input);
#endif
