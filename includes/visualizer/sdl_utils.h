#ifndef SDL_UTILS
#define SDL_UTILS

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <models/vector.h>

t_vec2	get_text_size(TTF_Text *text);
t_vec2	get_renderer_size(SDL_Renderer *renderer);

#endif