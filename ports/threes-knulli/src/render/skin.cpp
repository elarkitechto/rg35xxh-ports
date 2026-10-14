#include "render/skin.hpp"
#include "platform/sdl.hpp"
#include <string>

namespace triples::render {
SDL_Texture* load_skin(SDL_Renderer* renderer, const char* name, int width, int height) {
    char* base = SDL_GetBasePath();
    std::string path = std::string(base ? base : "./") + "assets/skin/" + name + ".rgba";
    SDL_free(base);
    size_t size = 0;
    void* bytes = SDL_LoadFile(path.c_str(), &size);
    if (!bytes) return nullptr;
    if (width <= 0 || height <= 0 || size != static_cast<size_t>(width) * height * 4) { SDL_free(bytes); return nullptr; }
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
    if (texture) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
        if (SDL_UpdateTexture(texture, nullptr, bytes, width * 4) != 0) {
            SDL_DestroyTexture(texture); texture = nullptr;
        }
    }
    SDL_free(bytes);
    return texture;
}
}
