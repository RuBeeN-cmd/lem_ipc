#include <visualizer/sdl.h>

int	handle_events(t_sdl *sdl)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			sdl_quit(sdl, 0);
		} else if (e.type == SDL_EVENT_KEY_DOWN) {
			sdl->on_key_down(e.key.key, sdl->on_key_down_data);
		} else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
			sdl->on_click((t_vec2) {e.button.x, e.button.y}, sdl->on_click_data);
		}
	}
	return (sdl->quit);
}

static int	set_renderer_size(t_sdl *sdl) {
	t_vec2	renderer_size;
	if (!SDL_GetCurrentRenderOutputSize(sdl->renderer, (int *) &renderer_size.x, (int *) &renderer_size.y))  {
		WARN("Failed to retrieve renderer size: %s\n", SDL_GetError());
		return (1);
	}
	sdl->renderer_size = (t_vec2) { renderer_size.x, renderer_size.y };
	return (0);
}

void	sdl_loop(t_sdl *sdl) {
	while (1) {
		if (handle_events(sdl))
			return ;
		set_renderer_size(sdl);
		sdl->update(sdl->update_data);
		SDL_Delay(sdl->refresh_delay);
	}
}
