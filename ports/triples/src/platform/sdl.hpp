#pragma once
// This fork uses SDL2: KNULLI's H700 driver is provided by its patched SDL2.
#include <SDL.h>

inline bool ts_init(Uint32 flags) { return SDL_Init(flags) == 0; }
inline SDL_Window* ts_window(const char* title, int w, int h, Uint32 flags) {
    return SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
}
inline SDL_Renderer* ts_renderer(SDL_Window* w, const char*) {
    SDL_Renderer* r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return r ? r : SDL_CreateRenderer(w, -1, SDL_RENDERER_SOFTWARE);
}
inline bool ts_safe_area(SDL_Window* w, SDL_Rect* rect) {
    rect->x = rect->y = 0;
    SDL_GetWindowSize(w, &rect->w, &rect->h);
    return true;
}
inline void ts_pixel_size(SDL_Window* w, int* width, int* height) {
    SDL_Renderer* r = SDL_GetRenderer(w);
    if (r) SDL_GetRendererOutputSize(r, width, height);
    else SDL_GetWindowSize(w, width, height);
}
inline float ts_density(SDL_Window* w) {
    int x, y, px, py;
    SDL_GetWindowSize(w, &x, &y);
    ts_pixel_size(w, &px, &py);
    return x > 0 ? float(px) / x : 1.0f;
}
inline Uint32 ts_mouse(float* x, float* y) {
    int ix, iy; Uint32 result = SDL_GetMouseState(&ix, &iy);
    *x = float(ix); *y = float(iy); return result;
}
inline int ts_texture(SDL_Renderer* r, SDL_Texture* t, const SDL_FRect* src, const SDL_FRect* dst) {
    SDL_Rect rect{};
    if (src) rect = {int(src->x), int(src->y), int(src->w), int(src->h)};
    return SDL_RenderCopyF(r, t, src ? &rect : nullptr, dst);
}
#define SDL_Init ts_init
#define SDL_CreateWindow ts_window
#define SDL_CreateRenderer ts_renderer
#define SDL_GetWindowSafeArea ts_safe_area
#define SDL_GetWindowSizeInPixels ts_pixel_size
#define SDL_GetWindowPixelDensity ts_density
#define SDL_GetMouseState ts_mouse
#define SDL_DestroyCursor SDL_FreeCursor
#define SDL_DestroySurface SDL_FreeSurface
#define SDL_RenderTexture ts_texture
#define SDL_RenderFillRect SDL_RenderFillRectF
#define SDL_GetRenderViewport SDL_RenderGetViewport
#define SDL_SCALEMODE_LINEAR SDL_ScaleModeLinear
#define SDL_SYSTEM_CURSOR_POINTER SDL_SYSTEM_CURSOR_HAND
#define SDL_WINDOW_HIGH_PIXEL_DENSITY SDL_WINDOW_ALLOW_HIGHDPI
#define SDL_EVENT_QUIT SDL_QUIT
#define SDL_EVENT_KEY_DOWN SDL_KEYDOWN
#define SDL_EVENT_MOUSE_BUTTON_DOWN SDL_MOUSEBUTTONDOWN
#define SDL_EVENT_MOUSE_BUTTON_UP SDL_MOUSEBUTTONUP
#define SDL_EVENT_MOUSE_MOTION SDL_MOUSEMOTION
#define SDL_EVENT_FINGER_DOWN SDL_FINGERDOWN
#define SDL_EVENT_FINGER_UP SDL_FINGERUP
#define SDL_EVENT_FINGER_MOTION SDL_FINGERMOTION
inline SDL_Surface* SDL_CreateSurfaceFrom(int w, int h, Uint32 fmt, void* data, int pitch) {
    return SDL_CreateRGBSurfaceWithFormatFrom(data, w, h, 32, pitch, fmt);
}
