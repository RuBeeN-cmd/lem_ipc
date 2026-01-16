#include <visualizer/sdl.h>

void destroy_sdl(t_sdl *sdl) {
	TTF_DestroyRendererTextEngine(sdl->text_engine);
	TTF_CloseFont(sdl->font);
	TTF_Quit();
	SDL_DestroyRenderer(sdl->renderer);
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}

int init_sdl(t_sdl *sdl, char title[], t_vec2 win_size) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		ERR("Failed to init SDL: %s\n", SDL_GetError());
		goto error;
	}
	if (!(sdl->window = SDL_CreateWindow(title, win_size.x, win_size.y, SDL_WINDOW_RESIZABLE))) {
		ERR("Failed to init window: %s\n", SDL_GetError());
		goto quit;
	}
	if (!(sdl->renderer = SDL_CreateRenderer(sdl->window, NULL))) {
		ERR("Failed to init renderer: %s\n", SDL_GetError());
		goto destroy_window;
	}
	if (!SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND)) {
		ERR("Failed to set blend mode: %s\n", SDL_GetError());
		goto destroy_window;
	}
	if (!TTF_Init()) {
		ERR("Failed to init SDL TTF: %s\n", SDL_GetError());
		goto destroy_renderer;
	}
	if (!(sdl->font = TTF_OpenFont(FONT_PATH, FONT_SIZE))) {
		ERR("Failed to open font: %s\n", SDL_GetError());
		goto ttf_quit;
	}
	if (!(sdl->text_engine = TTF_CreateRendererTextEngine(sdl->renderer))) {
		ERR("Failed to create text engine: %s\n", SDL_GetError());
		goto unload_font;
	}
	sdl->quit = 0;
	sdl->update = NULL;
	sdl->update_data = NULL;
	sdl->on_click = NULL;
	sdl->on_click_data = NULL;
	sdl->on_key_down = NULL;
	sdl->on_key_down_data = NULL;
	DBG("SDL successfully initialized\n");
	return (0);

	unload_font:		TTF_CloseFont(sdl->font);
	ttf_quit:			TTF_Quit();
	destroy_renderer:	SDL_DestroyRenderer(sdl->renderer);
	destroy_window:		SDL_DestroyWindow(sdl->window);
	quit:				SDL_Quit();
	error:				return (1);
}

void	set_update(t_sdl *sdl, void (*f)(void *), void *data, uint32_t refresh_delay) {
	sdl->update = f;
	sdl->update_data = data;
	sdl->refresh_delay = refresh_delay;
}

void	set_on_click(t_sdl *sdl, void (*f)(t_vec2, void *), void *data) {
	sdl->on_click = f;
	sdl->on_click_data = data;
}

void	set_on_key_down(t_sdl *sdl, void (*f)(SDL_Keycode, void *), void *data) {
	sdl->on_key_down = f;
	sdl->on_key_down_data = data;
}

void	sdl_quit(t_sdl *sdl, uint32_t delay) {
	if (delay)
		SDL_Delay(delay);
	sdl->quit = 1;
}