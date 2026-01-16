#ifndef SDL_H
#define SDL_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <utils/log.h>
#include <utils/color.h>
#include <utils/vector.h>

#define FONT_PATH	"./fonts/DejaVuSansMono.ttf"
#define FONT_SIZE	20.0

typedef struct	s_sdl
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	TTF_Font		*font;
	TTF_TextEngine	*text_engine;

	t_vec2			renderer_size;
	int				quit;

	void			(*update)(void *);
	void			*update_data;
	uint32_t		refresh_delay;

	void			(*on_click)(t_vec2, void *);
	void			*on_click_data;
	void			(*on_key_down)(SDL_Keycode, void *);
	void			*on_key_down_data;
}				t_sdl;

// sdl.c
int		init_sdl(t_sdl *sdl, char title[], t_vec2 win_size);
void	destroy_sdl(t_sdl *sdl);
void	sdl_quit(t_sdl *sdl, uint32_t delay);
void	set_update(t_sdl *sdl, void (*f)(void *), void *data, uint32_t refresh_delay);
void	set_on_click(t_sdl *sdl, void (*f)(t_vec2, void *), void *data);
void	set_on_key_down(t_sdl *sdl, void (*f)(SDL_Keycode, void *), void *data);

// loop.c
void	sdl_loop(t_sdl *sdl);
int		handle_events(t_sdl *sdl);

// utils.c
t_vec2	get_text_size(TTF_Text *text);

// draw.c
void	clear_window(t_sdl *sdl, t_color color);
void	draw_rectangle(t_sdl *sdl, t_vec2 pos, t_vec2 size, t_color color);
void	draw_border_rectangle(t_sdl *sdl, t_vec2 pos, t_vec2 size, t_color color);

#endif