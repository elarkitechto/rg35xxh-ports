#include "platform/sdl.hpp"
// Include once in the file with main() — on iOS this renames our `main` to
// `SDL_main` and supplies the real entry point that calls UIApplicationMain.
// On platforms that don't need a wrapper (Linux, macOS, Emscripten), the
// header is a no-op.
#include <SDL_main.h>

#include "app.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace {

triples::App* g_app = nullptr;

#ifdef __EMSCRIPTEN__
void web_tick() {
    if (g_app) g_app->tick();
}
#endif

}  // namespace

#ifdef __EMSCRIPTEN__
// Exported for the JS shell so it can initialize SDL3 audio from inside the
// first user-gesture stack frame, which iOS Safari requires for the
// AudioContext to start running instead of suspended.
extern "C" EMSCRIPTEN_KEEPALIVE void triples_init_audio() {
    if (g_app) g_app->init_audio_now();
}
#endif

int main(int /*argc*/, char* /*argv*/[]) {
    triples::App app;
    if (!app.initialize()) return 1;
    g_app = &app;

#ifdef __EMSCRIPTEN__
    // Let requestAnimationFrame drive the rate (0 = use rAF).
    emscripten_set_main_loop(web_tick, 0, 1);
    return 0;
#else
    int frame = 0;
    const bool smoke = SDL_getenv("THREES_SMOKE") != nullptr;
    const bool menu_smoke = SDL_getenv("THREES_MENU_SMOKE") != nullptr;
    const bool theme_smoke = SDL_getenv("THREES_THEME_SMOKE") != nullptr;
    const bool tate_menu_smoke = SDL_getenv("THREES_TATE_MENU_SMOKE") != nullptr;
    const bool tate_off_smoke = SDL_getenv("THREES_TATE_OFF_SMOKE") != nullptr;
    const bool sound_smoke = SDL_getenv("THREES_SOUND_SMOKE") != nullptr;
    const bool hotkey_smoke = SDL_getenv("THREES_HOTKEY_SMOKE") != nullptr;
    const bool stick_smoke = SDL_getenv("THREES_STICK_SMOKE") != nullptr;
    while (app.tick()) {
        if (smoke) {
            ++frame;
            if (stick_smoke) {
                if (frame == 10 || frame == 20 || frame == 30) {
                    SDL_Event e{};
                    e.type = SDL_JOYAXISMOTION;
                    e.jaxis.axis = 0; // left X on the raw joystick
                    e.jaxis.value = frame == 20 ? 0 : 24000;
                    SDL_PushEvent(&e);
                }
            } else if (hotkey_smoke) {
                if (frame == 10 || frame == 20) {
                    SDL_Event e{};
                    e.type = SDL_CONTROLLERBUTTONDOWN;
                    e.cbutton.button = frame == 10 ? SDL_CONTROLLER_BUTTON_GUIDE : SDL_CONTROLLER_BUTTON_START;
                    SDL_PushEvent(&e);
                }
            } else if (tate_off_smoke || sound_smoke) {
                SDL_Keycode key = 0;
                switch (frame) {
                    case 10: key = SDLK_TAB; break;
                    case 20: case 25: case 30: case 35: if (tate_off_smoke || frame == 20) key = SDLK_DOWN; break;
                    case 40: key = SDLK_RETURN; break;
                }
                if (key) { SDL_Event e{}; e.type = SDL_KEYDOWN; e.key.keysym.sym = key; SDL_PushEvent(&e); }
            } else if (tate_menu_smoke) {
                SDL_Keycode key = 0;
                switch (frame) {
                    case 10: key = SDLK_TAB; break;
                    case 20: case 25: case 30: case 35: key = SDLK_DOWN; break;
                    case 40: key = SDLK_RETURN; break;
                    case 50: key = SDLK_LEFT; break;
                }
                if (key) { SDL_Event e{}; e.type = SDL_KEYDOWN; e.key.keysym.sym = key; SDL_PushEvent(&e); }
            } else if (theme_smoke) {
                SDL_Keycode key = 0;
                switch (frame) {
                    case 10: key = SDLK_TAB; break;
                    case 20: case 25: case 30: key = SDLK_DOWN; break;
                    case 35: case 45: key = SDLK_RIGHT; break;
                    case 55: key = SDLK_TAB; break;
                }
                if (key) { SDL_Event e{}; e.type = SDL_KEYDOWN; e.key.keysym.sym = key; SDL_PushEvent(&e); }
            } else if (menu_smoke) {
                SDL_Keycode key = 0;
                switch (frame) {
                    case 10: key = SDLK_TAB; break;
                    case 20: key = SDLK_DOWN; break;
                    case 25: key = SDLK_LEFT; break;
                    case 30: key = SDLK_DOWN; break;
                    case 35: key = SDLK_RIGHT; break;
                    case 40: key = SDLK_DOWN; break;
                    case 45: key = SDLK_DOWN; break;
                    case 50: key = SDLK_RETURN; break;
                    case 55: key = SDLK_RIGHT; break;
                    case 65: key = SDLK_BACKSPACE; break;
                    case 70: key = SDLK_TAB; break;
                }
                if (key) { SDL_Event e{}; e.type = SDL_KEYDOWN; e.key.keysym.sym = key; SDL_PushEvent(&e); }
            } else if (frame == 10 || frame == 35 || frame == 60) {
                SDL_Event e{}; e.type = SDL_KEYDOWN;
                e.key.keysym.sym = frame == 10 ? SDLK_UP : frame == 35 ? SDLK_LEFT : SDLK_DOWN;
                SDL_PushEvent(&e);
            }
            SDL_Delay(16);
            if (hotkey_smoke && frame >= 30) return 2;
            if (frame >= 90) break;
        }
    }
    return 0;
#endif
}
