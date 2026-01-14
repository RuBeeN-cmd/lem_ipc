#include <visualizer/visualizer.h>

void	draw_pause_indicator(t_visualizer *v)
{
	uint32_t	renderer_width;
	uint32_t	renderer_height;
	SDL_GetCurrentRenderOutputSize(v->renderer, (int *) &renderer_width, (int *) &renderer_height);

	t_vec2	pos = {
		renderer_width / 2 - 50,
		0
	};
	draw_rectangle(v->renderer, (t_vec2) {0, 0}, (t_vec2) {renderer_width, 30}, color_from_u32(0x00000000));
	if (v->game_state & PAUSED) {
		TTF_Text *new_text = draw_text(v, "PAUSED", pos, color_from_u32(0xFFFFFFFF));
		TTF_DestroyText(new_text);
	}
}

void	draw_game(t_visualizer *v)
{
	clear_window(v->renderer, color_from_u32(0xFF000000));
	draw_board(v);
	draw_pause_indicator(v);
	draw_panel(v->renderer, &v->supervision_panel);
	draw_panel(v->renderer, &v->kills_panel);
	SDL_RenderPresent(v->renderer);
}