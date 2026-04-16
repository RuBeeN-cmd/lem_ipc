#include <visualizer/visualizer.h>

static void	init_visualizer_panels(t_visualizer *v)
{
	v->supervision_panel = init_panel(SUPERVISION_PANEL_SIZE, ANCHOR_TOP_RIGHT, 0);
	v->kills_panel = init_panel(KILLS_PANEL_SIZE, ANCHOR_TOP_LEFT, 1);
}

static int	init_visualizer(t_visualizer *v, char title[], uint32_t width, uint32_t height)
{
	t_vec2	board_size;

	if (init_visualizer_ipc(&v->ipc, &board_size) == 1)
		return (1);
	if (init_board_buffer(&v->board_copy, board_size))
		return (1);

	send_msg(v->ipc.msg_id, "*", 1, VISUALIZER_CHANNEL);
	v->board_size = board_size;
	v->cell_size = (height - INITIAL_PADDING) / v->board_size.y;
	if (v->cell_size * v->board_size.x > (width - INITIAL_PADDING))
		v->cell_size = (width - INITIAL_PADDING) / v->board_size.x;
	v->offset = (t_fvec2) {0, 0};
	v->target_infos = (t_supervised_infos) {
		.pos = NULL_POS,
		.team = -1,
		.is_alive = -1
	};
	v->kills = NULL;
	init_visualizer_panels(v);
	if (init_sdl(v, title, width, height))
	{
		free_board_buffer(v->board_copy, v->board_size.y);
		return (1);
	}
	return (0);
}

static void	destroy_visualizer(t_visualizer *v)
{
	shm_detach(v->ipc.data);
	check_msg(v->ipc.msg_id, NULL, 1, VISUALIZER_CHANNEL);
	destroy_text_line_list(&v->supervision_panel);
	destroy_text_line_list(&v->kills_panel);
	destroy_sdl(v);
	ft_lstclear(&v->kills, free);
	free_board_buffer(v->board_copy, v->board_size.y);
}

static int	visualizer_routine(t_visualizer *v)
{
	if (handle_events(v))
		return (0);
	update_supervision(v);
	update_kills(v);
	try_sync_shm(v);
	draw_game(v);
	if (!is_game_ended(v->board_copy, v->board_size))
		return (0);
	return (1);
}

static void	visualizer_loop(t_visualizer *v)
{
	while (visualizer_routine(v))
		SDL_Delay(12);
	SDL_Delay(2000);
}

int	visualizer_workflow(void)
{
	t_visualizer	v;

	if (init_visualizer(&v, WIN_TITLE, WIN_WIDTH, WIN_HEIGHT))
		return (1);
	visualizer_loop(&v);
	destroy_visualizer(&v);
	return (0);
}