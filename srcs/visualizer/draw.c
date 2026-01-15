#include <visualizer/visualizer.h>

void	draw_pause_screen(t_visualizer *v, t_vec2 renderer_size)
{
	uint32_t icon_offset = 10;
	t_vec2 icon_size = {40, 50};
	t_vec2 icon_bar_size = {icon_size.x / 3, icon_size.y};
	draw_border_rectangle(
		v->renderer,
		(t_vec2) {icon_offset, renderer_size.y - icon_offset - icon_size.y},
		icon_bar_size,
		color_from_u32(0xFF000000)
	);
	draw_rectangle(
		v->renderer,
		(t_vec2) {icon_offset + 1, renderer_size.y - icon_offset - icon_size.y + 1},
		sub_vec2(icon_bar_size, (t_vec2) {2, 2}),
		color_from_u32(0xFFFFFFFF)
	);
	draw_border_rectangle(
		v->renderer,
		(t_vec2) {icon_offset + icon_size.x - icon_bar_size.x, renderer_size.y - icon_offset - icon_size.y},
		icon_bar_size,
		color_from_u32(0xFF000000)
	);
	draw_rectangle(
		v->renderer,
		(t_vec2) {icon_offset + icon_size.x - icon_bar_size.x + 1, renderer_size.y - icon_offset - icon_size.y + 1},
		sub_vec2(icon_bar_size, (t_vec2) {2, 2}),
		color_from_u32(0xFFFFFFFF)
	);
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