#include <visualizer/visualizer.h>

static void	init_visualizer_panels(t_visualizer *v)
{
	v->supervision_panel = init_panel(SUPERVISION_PANEL_SIZE, ANCHOR_TOP_RIGHT, 0);
	v->kills_panel = init_panel(KILLS_PANEL_SIZE, ANCHOR_TOP_LEFT, 1);
}

static void	visualizer_routine(void *_v)
{
	t_visualizer *v = _v;
	update_supervision(v);
	update_kills(v);
	try_sync_shm(v);
	if (!is_game_ended(v->board_copy, v->board_size))
		sdl_quit(&v->sdl, 2000);
	draw_game(v);
}

static int	init_visualizer(t_visualizer *v, char title[], t_vec2 win_size)
{
	t_vec2	board_size;

	if (init_visualizer_ipc(&v->ipc, &board_size) == 1)
		return (1);
	if (init_board_copy(&v->board_copy, board_size))
		return (1);
	send_msg(v->ipc.msg_id, "*", 1, VISUALIZER_CHANNEL);

	v->board_size = board_size;
	v->cell_size = (win_size.y - INITIAL_PADDING) / v->board_size.y;
	if (v->cell_size * v->board_size.x > (uint32_t) (win_size.x - INITIAL_PADDING))
		v->cell_size = (win_size.x - INITIAL_PADDING) / v->board_size.x;
	v->offset = (t_fvec2) {0, 0};
	v->target_infos = (t_supervised_infos) {
		.pos = NULL_POS,
		.team = -1,
		.is_alive = 0
	};
	v->kills = NULL;
	init_visualizer_panels(v);
	if (init_sdl(&v->sdl, title, win_size)) {
		free_board_copy(v->board_copy, v->board_size.y);
		return (1);
	}

	set_update(&v->sdl, visualizer_routine, v, REFRESH_DELAY);
	set_on_click(&v->sdl, on_click, v);
	set_on_key_down(&v->sdl, on_key_down, v);
	return (0);
}

static void	destroy_visualizer(t_visualizer *v)
{
	shm_detach(v->ipc.data);
	check_msg(v->ipc.msg_id, NULL, 1, VISUALIZER_CHANNEL);
	destroy_text_line_list(&v->supervision_panel);
	destroy_text_line_list(&v->kills_panel);
	destroy_sdl(&v->sdl);
	ft_lstclear(&v->kills, free);
	free_board_copy(v->board_copy, v->board_size.y);
}

int	visualizer_workflow(void)
{
	t_visualizer	v;

	if (init_visualizer(&v, WIN_TITLE, WIN_SIZE))
		return (1);
	sdl_loop(&v.sdl);
	destroy_visualizer(&v);
	return (0);
}