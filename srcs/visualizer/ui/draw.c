#include <visualizer/visualizer.h>

void	draw_cell(t_visualizer *v, int x, int y, int team, uint32_t renderer_width, uint32_t renderer_height)
{
	t_color		team_color = get_team_color(team);
	size_t		cell_size = v->cell_size;
	t_vec2		pos = {
		(x - v->board_size.x / 2.0 - (float) v->offset.x) * (float) cell_size + renderer_width / 2.0 + BORDER_WIDTH / 2.0,
		(y - v->board_size.y / 2.0 - (float) v->offset.y) * (float) cell_size + renderer_height / 2.0 + BORDER_WIDTH / 2.0
	};
	if (pos.x < 0)
		pos.x--;
	if (pos.y < 0)
		pos.y--;
	SDL_FRect	cell = {pos.x, pos.y, cell_size - BORDER_WIDTH, cell_size - BORDER_WIDTH};
	SDL_SetRenderDrawColor(v->sdl.renderer, team_color.r, team_color.g, team_color.b, team_color.a);
	SDL_RenderFillRect(v->sdl.renderer, &cell);
	if (v->target_infos.is_alive && !vec2cmp(v->target_infos.pos, (t_vec2) {x, y}))
	{
		t_vec2	pin_rect_size = {cell_size - BORDER_WIDTH, cell_size - BORDER_WIDTH};
		pin_rect_size = scalar_div_vec2(pin_rect_size, 2);
		t_vec2	pin_rect_pos = add_vec2(pos, scalar_div_vec2(pin_rect_size, 2));
		SDL_FRect	cell = {pin_rect_pos.x, pin_rect_pos.y, pin_rect_size.x, pin_rect_size.y};
		SDL_SetRenderDrawColor(v->sdl.renderer, 0, 0, 0, 0xFF);
		SDL_RenderFillRect(v->sdl.renderer, &cell);
	}
}

void	draw_board(t_visualizer *v)
{
	for (size_t y = 0; y < (size_t) v->board_size.y; y++)
		for (size_t x = 0; x < (size_t) v->board_size.x; x++)
			draw_cell(v, x, y, v->board_copy[y][x], v->sdl.renderer_size.x, v->sdl.renderer_size.y);
}

TTF_Text	*draw_text(t_visualizer *v, char *text, t_vec2 pos, t_color color) {
	TTF_Text *ttf_text = TTF_CreateText(v->sdl.text_engine, v->sdl.font, text, ft_strlen(text));
	if (!ttf_text) {
		ft_printf_fd(2, "%s\n", SDL_GetError());
		return (NULL);
	}
	if (!TTF_SetTextColor(ttf_text, color.r, color.g, color.b, color.a))
	{
		ft_printf_fd(2, "%s\n", SDL_GetError());
		TTF_DestroyText(ttf_text);
		return (NULL);
	}
	if (!TTF_DrawRendererText(ttf_text, pos.x, pos.y))
	{
		ft_printf_fd(2, "%s\n", SDL_GetError());
		TTF_DestroyText(ttf_text);
		return (NULL);
	}
	return (ttf_text);
}

void	draw_pause_screen(t_visualizer *v)
{
	uint32_t icon_offset = 10;
	t_vec2 icon_size = {40, 50};
	t_vec2 icon_bar_size = {icon_size.x / 3, icon_size.y};
	draw_border_rectangle(
		&v->sdl,
		(t_vec2) {icon_offset, v->sdl.renderer_size.y - icon_offset - icon_size.y},
		icon_bar_size,
		color_from_u32(0xFF000000)
	);
	draw_rectangle(
		&v->sdl,
		(t_vec2) {icon_offset + 1, v->sdl.renderer_size.y - icon_offset - icon_size.y + 1},
		sub_vec2(icon_bar_size, (t_vec2) {2, 2}),
		color_from_u32(0xFFFFFFFF)
	);
	draw_border_rectangle(
		&v->sdl,
		(t_vec2) {icon_offset + icon_size.x - icon_bar_size.x, v->sdl.renderer_size.y - icon_offset - icon_size.y},
		icon_bar_size,
		color_from_u32(0xFF000000)
	);
	draw_rectangle(
		&v->sdl,
		(t_vec2) {icon_offset + icon_size.x - icon_bar_size.x + 1, v->sdl.renderer_size.y - icon_offset - icon_size.y + 1},
		sub_vec2(icon_bar_size, (t_vec2) {2, 2}),
		color_from_u32(0xFFFFFFFF)
	);
}

int	draw_game(t_visualizer *v)
{
	clear_window(&v->sdl, color_from_u32(0xFF000000));
	draw_board(v);
	draw_panel(&v->sdl, &v->supervision_panel);
	draw_panel(&v->sdl, &v->kills_panel);
	if (v->shm_copy.game_state & PAUSED)
		draw_pause_screen(v);
	SDL_RenderPresent(v->sdl.renderer);
	return (0);
}