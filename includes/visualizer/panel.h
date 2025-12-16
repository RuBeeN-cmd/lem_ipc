#ifndef PANEL_H
#define PANEL_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <models/color.h>
#include <models/vector.h>
#include <libft.h>

#define PANEL_DEF_MARGIN		10
#define PANEL_DEF_PADDING		10
#define PANEL_DEF_BORDER_WIDTH	2

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

typedef struct s_button
{
	t_vec2	position;
	t_vec2	size;
	t_color	color;
	int		border_size;
	int		border_color;
	void	(*on_click)(void);
} 				t_button;

typedef enum e_element_type
{
	ELEMENT_TYPE_BUTTON,
	ELEMENT_TYPE_TEXT_LINE
}				t_element_type;

typedef struct s_ui_element
{
	t_element_type	type;
	void			*element;
}				t_ui_element;

typedef struct s_ui_row
{
	uint32_t	line_number;
	t_list		*elements;
}				t_ui_row;

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

	t_list		*rows;
}				t_panel;


t_panel	init_panel(t_vec2 size, t_anchor anchor);
void	draw_panel(SDL_Renderer *renderer, t_panel *panel);
void	destroy_text_line_list(t_panel *panel);
int		add_text_line(TTF_TextEngine *engine, TTF_Font *font, t_panel *panel, char *text, t_color color, t_justification justification, uint32_t line_number);

#endif