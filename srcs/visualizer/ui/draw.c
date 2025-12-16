#include <visualizer/visualizer.h>

void	draw_cell(t_visualizer *v, int x, int y, int team, uint32_t renderer_width, uint32_t renderer_height)
{
	t_color		team_color = get_team_color(team);
	size_t		cell_size = v->cell_size;
	t_vec2		pos = {
		(x - (float) (v->board_size.x / 2) - v->offset.x) * cell_size + renderer_width / 2 + BORDER_WIDTH / 2,
		(y - (float) (v->board_size.y / 2) - v->offset.y) * cell_size + renderer_height / 2 + BORDER_WIDTH / 2
	};
	SDL_FRect	cell = {pos.x, pos.y, cell_size - BORDER_WIDTH, cell_size - BORDER_WIDTH};
	SDL_SetRenderDrawColor(v->renderer, team_color.r, team_color.g, team_color.b, team_color.a);
	SDL_RenderFillRect(v->renderer, &cell);
	if (v->target_infos.is_alive && !vec2cmp(v->target_infos.pos, (t_vec2) {x, y}))
	{
		t_vec2	pin_rect_size = {cell_size - BORDER_WIDTH, cell_size - BORDER_WIDTH};
		pin_rect_size = scalar_div_vec2(pin_rect_size, 2);
		t_vec2	pin_rect_pos = add_vec2(pos, scalar_div_vec2(pin_rect_size, 2));
		SDL_FRect	cell = {pin_rect_pos.x, pin_rect_pos.y, pin_rect_size.x, pin_rect_size.y};
		SDL_SetRenderDrawColor(v->renderer, 0, 0, 0, 0xFF);
		SDL_RenderFillRect(v->renderer, &cell);
	}
}

void	draw_board(t_visualizer *v)
{
	uint32_t renderer_width;
	uint32_t renderer_height;
	SDL_GetCurrentRenderOutputSize(v->renderer, (int *) &renderer_width, (int *) &renderer_height);
	for (size_t y = 0; y < (size_t) v->board_size.y; y++)
		for (size_t x = 0; x < (size_t) v->board_size.x; x++)
			draw_cell(v, x, y, v->buffer[y][x], renderer_width, renderer_height);
}

void	clear_window(SDL_Renderer *renderer, t_color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer);
}

TTF_Text	*draw_text(t_visualizer *v, char *text, t_vec2 pos, t_color color) {
	TTF_Text *ttf_text = TTF_CreateText(v->text_engine, v->font, text, ft_strlen(text));
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

// void	draw_target_infos(t_visualizer *v, t_supervised_infos *target_infos)
// {
// 	if (!v->supervision_panel.visible)
// 	return ;
// 	// uint32_t renderer_width;
// 	// uint32_t renderer_height;
// 	// SDL_GetCurrentRenderOutputSize(v->renderer, (int *) &renderer_width, (int *) &renderer_height);
	
	



// 	draw_panel(v->renderer, &v->supervision_panel);



// 	// (void) target_infos;



// 	// t_vec2 pos = (t_vec2) {v->supervision_panel.offset.x + 10, v->supervision_panel.offset.y + 10};

// 	// TTF_Text *text = draw_text(v, "Target Info : ", pos, color_from_u32(0xFF000000));
// 	// t_vec2 text_size = get_text_size(text);
// 	// pos.y += text_size.y;
// 	// TTF_DestroyText(text);
// 	// text = draw_text(v, "Position : ", pos, color_from_u32(0xFF000000));
// 	// text_size = get_text_size(text);
// 	// pos.x += text_size.x + 10;
// 	// TTF_DestroyText(text);
// 	// char *str = ft_itoa(v->target_infos.pos.x);
// 	// str = ft_strjoin_free(str, ", ", 1);
// 	// str = ft_strjoin_free(str, ft_itoa(v->target_infos.pos.y), 3);
// 	// text = draw_text(v, str, pos, color_from_u32(0xFF000000));
// 	// TTF_DestroyText(text);
// 	// free(str);
// }