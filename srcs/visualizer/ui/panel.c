#include <visualizer/panel.h>

t_panel	init_panel(t_vec2 size, t_anchor anchor, int visible)
{
	return ((t_panel) {
		.color = color_from_u32(0xFFFFFFFF),
		.size = size,
		.anchor = anchor,
		.margin = (t_vec2) {PANEL_DEF_MARGIN, PANEL_DEF_MARGIN},
		.padding = (t_vec2) {PANEL_DEF_PADDING, PANEL_DEF_PADDING},
		.visible = visible,
		.border_size = PANEL_DEF_BORDER_WIDTH,
		.border_color = color_from_u32(0xFF000000),
		.text_line_list = NULL,
		.rows = NULL
	});
}

void	draw_rectangle(SDL_Renderer *renderer, t_vec2 pos, t_vec2 size, t_color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_FRect rect = {pos.x, pos.y, size.x, size.y};
	SDL_RenderFillRect(renderer, &rect);
}

void	draw_panel(SDL_Renderer *renderer, t_panel *panel)
{
	if (!panel->visible)
		return ;
	uint32_t renderer_width;
	uint32_t renderer_height;
	SDL_GetCurrentRenderOutputSize(renderer, (int *) &renderer_width, (int *) &renderer_height);

	t_vec2 pos = {0, 0};
	t_vec2 size = panel->size;
	switch (panel->anchor)
	{
		case ANCHOR_TOP_LEFT:
			pos = panel->margin;
			break;
		case ANCHOR_TOP_RIGHT:
			pos = (t_vec2) {renderer_width - panel->size.x - panel->margin.x, panel->margin.y};
			break;
		case ANCHOR_BOTTOM_LEFT:
			pos = (t_vec2) {panel->margin.x, renderer_height - panel->size.y - panel->margin.y};
			break;
		case ANCHOR_BOTTOM_RIGHT:
			pos = (t_vec2) {renderer_width - panel->size.x - panel->margin.x, renderer_height - panel->size.y - panel->margin.y};
			break;
		case ANCHOR_CENTER:
			pos = (t_vec2) {renderer_width / 2 - panel->size.x / 2, renderer_height / 2 - panel->size.y / 2};
			break;
		default:
			break;
	}
	draw_rectangle(renderer, pos, size, panel->border_color);

	pos = add_vec2(pos, (t_vec2) {panel->border_size, panel->border_size});
	size = sub_vec2(panel->size, scalar_mult_vec2((t_vec2) {panel->border_size, panel->border_size}, 2));
	draw_rectangle(renderer, pos, size, panel->color);

	if (panel->text_line_list)
	{
		t_list *current = panel->text_line_list;
		t_text_line *first_line = current->content;
		int pos_y = pos.y + panel->padding.y + first_line->size.y * first_line->line_number;
		while (current)
		{
			t_text_line *line = current->content;
			current = current->next;
			TTF_SetTextColor(line->ttf_text, line->color.r, line->color.g, line->color.b, line->color.a);
			int pos_x = 0;
			switch (line->justification)
			{
				case JUSTIFY_LEFT:
					pos_x = pos.x + panel->padding.x;
					break;
				case JUSTIFY_CENTER:
					pos_x = pos.x + (size.x - line->size.x) / 2;
					break;
				case JUSTIFY_RIGHT:
					pos_x = pos.x + size.x - line->size.x - 10;
					break;
				default:
					break;
			}
			TTF_DrawRendererText(line->ttf_text, pos_x, pos_y);
			t_text_line *next_line = current ? current->content : NULL;
			if (next_line && next_line->line_number != line->line_number)
				pos_y += line->size.y * (next_line->line_number - line->line_number);
		}
	}
}