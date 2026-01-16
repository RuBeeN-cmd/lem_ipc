#include <visualizer/panel.h>

t_panel	init_panel(t_vec2 size, t_anchor anchor, int visible)
{
	return ((t_panel) {
		.color = color_from_u32(0xAFFFFFFF),
		.size = size,
		.anchor = anchor,
		.margin = (t_vec2) {PANEL_DEF_MARGIN, PANEL_DEF_MARGIN},
		.padding = (t_vec2) {PANEL_DEF_PADDING, PANEL_DEF_PADDING},
		.visible = visible,
		.border_size = PANEL_DEF_BORDER_WIDTH,
		.border_color = color_from_u32(0xFF000000),
		.text_line_list = NULL
	});
}

static t_vec2	get_panel_pos(t_sdl *sdl, t_panel *panel) {
	switch (panel->anchor)
	{
		case ANCHOR_TOP_LEFT:
			return panel->margin;
		case ANCHOR_TOP_RIGHT:
			return (t_vec2) { sdl->renderer_size.x - panel->size.x - panel->margin.x, panel->margin.y };
		case ANCHOR_BOTTOM_LEFT:
			return (t_vec2) { panel->margin.x, sdl->renderer_size.y - panel->size.y - panel->margin.y };
		case ANCHOR_BOTTOM_RIGHT:
			return (t_vec2) { sdl->renderer_size.x - panel->size.x - panel->margin.x, sdl->renderer_size.y - panel->size.y - panel->margin.y };
		case ANCHOR_CENTER:
			return (t_vec2) { sdl->renderer_size.x / 2 - panel->size.x / 2, sdl->renderer_size.y / 2 - panel->size.y / 2 };
		default:
			return (t_vec2) { 0, 0 };
	}
}

static void	draw_panel_text(t_panel *panel, t_vec2 pos, t_vec2 size) {
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

void	draw_panel(t_sdl *sdl, t_panel *panel)
{
	if (!panel->visible)
		return ;
	t_vec2 pos = get_panel_pos(sdl, panel);
	t_vec2 size = panel->size;

	draw_border_rectangle(sdl, pos, size, panel->border_color);
	pos = add_vec2(pos, (t_vec2) {panel->border_size, panel->border_size});
	size = sub_vec2(panel->size, scalar_mult_vec2((t_vec2) {panel->border_size, panel->border_size}, 2));
	draw_rectangle(sdl, pos, size, panel->color);
	draw_panel_text(panel, pos, size);

}