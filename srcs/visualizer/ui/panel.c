#include <visualizer/visualizer.h>

t_panel	init_panel(t_vec2 size, t_anchor anchor)
{
	return ((t_panel) {
		.color = color_from_u32(0xFFFFFFFF),
		.size = size,
		.anchor = anchor,
		.margin = (t_vec2) {PANEL_DEF_MARGIN, PANEL_DEF_MARGIN},
		.padding = (t_vec2) {PANEL_DEF_PADDING, PANEL_DEF_PADDING},
		.visible = 1,
		.border_size = PANEL_DEF_BORDER_WIDTH,
		.border_color = color_from_u32(0xFF000000),
		.text_line_list = NULL,
	});
}

static void	destroy_text_line(void *content)
{
	t_text_line	*line;

	line = (t_text_line *) content;
	if (line->text)
		free(line->text);
	if (line->ttf_text)
		TTF_DestroyText(line->ttf_text);
	free(line);
}

void	destroy_text_line_list(t_panel *panel)
{
	ft_lstclear(&panel->text_line_list, destroy_text_line);
}

static t_vec2	get_text_size(TTF_Text *text)
{
	t_vec2 size;
	if (!TTF_GetTextSize(text, &size.x, &size.y))
	{
		ft_printf_fd(2, "%s\n", SDL_GetError());
		return (NULL_POS);
	}
	return (size);
}

int	add_text_line(TTF_TextEngine *engine, TTF_Font *font, t_panel *panel, char *text, t_color color, t_justification justification, uint32_t line_number)
{
	t_text_line	*line;
	t_list		*new_node;

	line = malloc(sizeof(t_text_line));
	if (!line)
		return (1);
	line->text = ft_strdup(text);
	line->color = color;
	line->justification = justification;
	line->line_number = line_number;
	line->ttf_text = TTF_CreateText(engine, font, line->text, ft_strlen(line->text));
	if (!line->ttf_text)
	{
		free(line->text);
		free(line);
		return (1);
	}
	line->size = get_text_size(line->ttf_text);
	if (!vec2cmp(line->size, NULL_POS))
	{
		TTF_DestroyText(line->ttf_text);
		free(line->text);
		free(line);
		return (1);
	}
	new_node = ft_lstnew(line);
	if (!new_node)
	{
		free(line->text);
		free(line);
		return (1);
	}
	ft_lstadd_back(&panel->text_line_list, new_node);
	return (0);
}

void	draw_panel(SDL_Renderer *renderer, t_panel *panel)
{
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
	SDL_SetRenderDrawColor(renderer, panel->border_color.r, panel->border_color.g, panel->border_color.b, panel->border_color.a);
	SDL_FRect rect = {pos.x, pos.y, size.x, size.y};
	SDL_RenderFillRect(renderer, &rect);
	pos = add_vec2(pos, (t_vec2) {panel->border_size, panel->border_size});
	size = sub_vec2(panel->size, scalar_mult_vec2((t_vec2) {panel->border_size, panel->border_size}, 2));
	SDL_SetRenderDrawColor(renderer, panel->color.r, panel->color.g, panel->color.b, panel->color.a);
	rect = (SDL_FRect) {pos.x, pos.y, size.x, size.y};
	SDL_RenderFillRect(renderer, &rect);
	// Draw Text
	if (panel->text_line_list)
	{
		int pos_y = pos.y + panel->padding.y;
		t_list *current = panel->text_line_list;
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
			if (current && ((t_text_line *) current->content)->line_number != line->line_number)
				pos_y += line->size.y;
		}
	}
}