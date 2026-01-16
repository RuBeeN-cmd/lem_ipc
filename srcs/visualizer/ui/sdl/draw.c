#include <visualizer/sdl.h>

void	clear_window(t_sdl *sdl, t_color color)
{
	SDL_SetRenderDrawColor(sdl->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(sdl->renderer);
}

void	draw_rectangle(t_sdl *sdl, t_vec2 pos, t_vec2 size, t_color color)
{
	SDL_SetRenderDrawColor(sdl->renderer, color.r, color.g, color.b, color.a);
	SDL_FRect rect = {pos.x, pos.y, size.x, size.y};
	SDL_RenderFillRect(sdl->renderer, &rect);
}

void	draw_border_rectangle(t_sdl *sdl, t_vec2 pos, t_vec2 size, t_color color)
{
	SDL_SetRenderDrawColor(sdl->renderer, color.r, color.g, color.b, color.a);
	SDL_FRect rect = {pos.x, pos.y, size.x, size.y};
	SDL_RenderRect(sdl->renderer, &rect);
}