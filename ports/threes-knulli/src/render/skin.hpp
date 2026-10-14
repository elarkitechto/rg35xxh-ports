#pragma once
struct SDL_Renderer;
struct SDL_Texture;
namespace triples::render {
SDL_Texture* load_skin(SDL_Renderer* renderer, const char* name, int width = 100, int height = 150);
}
