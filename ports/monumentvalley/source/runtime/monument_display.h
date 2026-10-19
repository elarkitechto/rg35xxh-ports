#ifndef MONUMENT_DISPLAY_H
#define MONUMENT_DISPLAY_H
struct monument_target;
void monument_display_init(int width, int height, int tate);
int monument_display_width(void);
int monument_display_height(void);
int monument_display_tate(void);
int monument_display_landscape(void);
void monument_display_request_resize(void);
void monument_display_toggle(void);
int monument_display_take_resize(void);
void monument_display_context(struct monument_target **slot, int is_window);
void monument_display_bind_framebuffer(unsigned target, unsigned framebuffer);
void monument_display_present(void);
#endif
