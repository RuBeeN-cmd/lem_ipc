#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <lem_ipc.h>
#include <ipc.h>
#include <models/color.h>
#include <models/vector.h>
#include <visualizer/panel.h>

#define WIN_WIDTH					800
#define WIN_HEIGHT					600
#define WIN_TITLE					"Lem-Ipc"
#define SUPERVISION_PANEL_WIDTH		300
#define SUPERVISION_PANEL_HEIGHT	140
#define SUPERVISION_PANEL_SIZE		((t_vec2) {SUPERVISION_PANEL_WIDTH, SUPERVISION_PANEL_HEIGHT})
#define KILLS_PANEL_WIDTH			200
#define KILLS_PANEL_HEIGHT			300
#define KILLS_PANEL_SIZE			((t_vec2) {KILLS_PANEL_WIDTH, KILLS_PANEL_HEIGHT})
#define FONT_PATH					"./fonts/DejaVuSansMono.ttf"

#define INITIAL_PADDING		20
#define BORDER_WIDTH		1
#define MOVE_SPEED			12
#define ZOOM_SPEED			1

#define MAX_LEADERBOARD		10

typedef struct	s_visualizer
{
	SDL_Window			*window;
	SDL_Renderer		*renderer;
	TTF_Font			*font;
	TTF_TextEngine		*text_engine;
	t_panel				supervision_panel;
	t_panel				kills_panel;

	t_fvec2				offset;
	uint32_t			cell_size;
	
	t_ipc				ipc;
	t_shm_data			shm_copy;
	uint32_t			**board_copy;
	t_vec2				board_size;
	t_list				*kills;
	t_supervised_infos	target_infos;
}				t_visualizer;

// visualizer.c
int		visualizer_workflow(void);

// ipc.c
void	try_sync_shm(t_visualizer *v);

// draw.c
void	draw_pause_indicator(t_visualizer *v);
void	draw_game(t_visualizer *v);

// events.c
int		handle_events(t_visualizer* v);

// buffer.c
void	free_board_buffer(uint32_t **buffer, size_t height);
int		init_board_buffer(uint32_t ***buffer, t_vec2 board_size);
void	copy_board(uint32_t **dst, uint32_t *src, t_vec2 board_size);

// sdl.c
int			init_sdl(t_visualizer *v, char title[], uint32_t width, uint32_t height);
void		destroy_sdl(t_visualizer *v);
TTF_Font	*load_font(const char *path, int32_t font_size);
void		unload_font(TTF_Font *font);

// draw.c
void		draw_board(t_visualizer *v);
void		clear_window(SDL_Renderer *renderer, t_color color);
TTF_Text	*draw_text(t_visualizer *v, char *text, t_vec2 pos, t_color color);

// kills.c
void 	update_kills(t_visualizer *v);

// supervision.c
void	update_supervision(t_visualizer *v);

// utils.c
int	is_game_ended(uint32_t **board, t_vec2 board_size);

#endif