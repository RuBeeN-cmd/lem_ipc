#ifndef PANEL_H
#define PANEL_H

#include <visualizer/sdl.h>

#include <utils/color.h>
#include <utils/vector.h>
#include <libft.h>

#define PANEL_DEF_MARGIN		10
#define PANEL_DEF_PADDING		10
#define PANEL_DEF_BORDER_WIDTH	1

typedef enum e_justification
{
	JUSTIFY_LEFT,
	JUSTIFY_RIGHT,
	JUSTIFY_CENTER
}				t_justification;

typedef struct s_text_line
{
	char					*text;
	TTF_Text				*ttf_text;
	t_vec2					size;
	t_color					color;
	uint32_t				line_number;
	t_justification			justification;
}				t_text_line;
typedef enum e_anchor

{
	ANCHOR_TOP_LEFT,
	ANCHOR_TOP_RIGHT,
	ANCHOR_BOTTOM_LEFT,
	ANCHOR_BOTTOM_RIGHT,
	ANCHOR_CENTER
}				t_anchor;

typedef struct	s_panel
{
	t_vec2		size;
	int			visible;
	t_color		color;
	int			border_size;
	t_color		border_color;
	t_anchor	anchor;
	t_vec2		margin;
	t_vec2		padding;
	t_list		*text_line_list;
}				t_panel;


// panel.c
t_panel	init_panel(t_vec2 size, t_anchor anchor, int visible);
void	draw_panel(t_sdl *sdl, t_panel *panel);

// text_line.c
void	destroy_text_line_list(t_panel *panel);
t_list	*add_text_line(TTF_TextEngine *engine, TTF_Font *font, t_panel *panel, char *text, t_color color, t_justification justification, uint32_t line_number);

#endif