#include <visualizer/panel.h>

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

static t_text_line	*init_text_line(char *text, t_color color, t_justification justification, uint32_t line_number, TTF_Text *ttf_text)
{
	t_vec2 text_size = get_text_size(ttf_text);
	if (!vec2cmp(text_size, NULL_SIZE))
		return (NULL);
	t_text_line	*line = malloc(sizeof(t_text_line));
	if (!line)
		return (NULL);
	line->text = ft_strdup(text);
	if (!line->text)
	{
		free(line);
		return (NULL);
	}
	line->color = color;
	line->justification = justification;
	line->line_number = line_number;
	line->ttf_text = ttf_text;
	line->size = text_size;
	return (line);
}

static int	cmp_text_line(void *a, void *b)
{
	t_text_line	*line_a;
	t_text_line	*line_b;

	line_a = (t_text_line *) a;
	line_b = (t_text_line *) b;
	if (line_a->line_number < line_b->line_number)
		return (-1);
	else if (line_a->line_number > line_b->line_number)
		return (1);
	return (0);
}

t_list	*add_text_line(TTF_TextEngine *engine, TTF_Font *font, t_panel *panel, char *text, t_color color, t_justification justification, uint32_t line_number)
{
	t_list		*new_node;
	TTF_Text	*ttf_text;
	t_text_line	*line;

	if (!(ttf_text = TTF_CreateText(engine, font, text, ft_strlen(text))))
		return (NULL);
	if (!(line = init_text_line(text, color, justification, line_number, ttf_text)))
	{
		TTF_DestroyText(ttf_text);
		return (NULL);
	}
	if (!(new_node = ft_lstnew(line)))
	{
		destroy_text_line(line);
		return (NULL);
	}
	ft_lstadd_back(&panel->text_line_list, new_node);
	ft_lstsort(panel->text_line_list, cmp_text_line);
	return (new_node);
}