#ifndef THREES_DISPLAY_H
#define THREES_DISPLAY_H
struct threes_target;
void threes_display_init(int width, int height, int tate);
int threes_display_width(void);
int threes_display_height(void);
int threes_display_tate(void);
void threes_display_toggle(void);
int threes_display_take_resize(void);
void threes_display_context(struct threes_target **slot, int is_window);
void threes_display_bind_framebuffer(unsigned target, unsigned framebuffer);
void threes_display_present(void);
#endif
