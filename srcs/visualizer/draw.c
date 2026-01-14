#include <visualizer/visualizer.h>

void	draw_pause_screen(t_visualizer *v, t_vec2 renderer_size)
{
	draw_rectangle(v->renderer, (t_vec2) {0, 0}, renderer_size, color_from_u32(0xCF000000));
	TTF_Text *text = draw_text(v, "PAUSED", scalar_div_vec2(renderer_size, 2), color_from_u32(0xFFFFFFFF));
	TTF_DestroyText(text);
}

int	draw_game(t_visualizer *v)
{
	t_vec2	renderer_size = get_renderer_size(v->renderer);
	if (!vec2cmp(renderer_size, NULL_SIZE)) {
		ERR("Failed to retrieve current renderer size: %s\n", SDL_GetError());
		return (1);
	}

	clear_window(v->renderer, color_from_u32(0xFF000000));
	draw_board(v);
	draw_panel(v->renderer, &v->supervision_panel);
	draw_panel(v->renderer, &v->kills_panel);
	if (v->shm_copy.game_state & PAUSED)
		draw_pause_screen(v, renderer_size);
	SDL_RenderPresent(v->renderer);
	return (0);
}