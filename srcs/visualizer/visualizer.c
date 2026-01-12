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
	if (init_buffer(&v->buffer, board_size))
		return (1);

	send_msg(v->ipc.msg_id, "*", 1, VISUALIZER_CHANNEL);
	v->running = -1;
	v->board_size = board_size;
	v->cell_size = (height - INITIAL_PADDING) / v->board_size.y;
	if (v->cell_size * v->board_size.x > (width - INITIAL_PADDING))
		v->cell_size = (width - INITIAL_PADDING) / v->board_size.x;
	v->offset = (t_fvec2) {0, 0};
	v->target_infos = (t_supervised_infos) {
		.pos = NULL_POS,
		.team = -1,
		.is_alive = 0
	};
	v->kills = NULL;
	init_visualizer_panels(v);
	if (init_sdl(v, title, width, height))
	{
		free_buffer(v->buffer, v->board_size.y);
		return (1);
	}
	return (0);
}

static void	destroy_visualizer(t_visualizer *v)
{
	check_msg(v->ipc.msg_id, NULL, 1, VISUALIZER_CHANNEL);
	destroy_text_line_list(&v->supervision_panel);
	destroy_text_line_list(&v->kills_panel);
	destroy_sdl(v);
	ft_lstclear(&v->kills, free);
	free_buffer(v->buffer, v->board_size.y);
}

static void	update_supervision(t_visualizer *v)
{
	check_msg(v->ipc.msg_id, &v->target_infos, sizeof(v->target_infos), TARGET_INFOS_CHANNEL);
	create_supervision_panel_text_lines(v, &v->supervision_panel, v->target_infos);
}

static void update_kills(t_visualizer *v)
{
	update_kill_list(v);
	create_kills_panel_text_lines(v, &v->kills_panel);
}

static void try_copy_board(t_visualizer *v)
{
	if (sem_lock_no_wait(v->ipc.sem_id) != -1)
	{
		copy_buffer(v->buffer, v->ipc.data, v->board_size);
		sem_unlock(v->ipc.sem_id);
	}
}

static void	draw_game(t_visualizer *v)
{
	clear_window(v->renderer, color_from_u32(0xFF000000));
	draw_board(v);
	draw_panel(v->renderer, &v->supervision_panel);
	draw_panel(v->renderer, &v->kills_panel);
	SDL_RenderPresent(v->renderer);
}

static int	visualizer_routine(t_visualizer *v)
{
	if (handle_events(v))
		return (0);
	update_supervision(v);
	update_kills(v);
	try_copy_board(v);
	draw_game(v);
	if (!is_game_ended(v->buffer, v->board_size))
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
	shm_det(v.ipc.data);
	destroy_visualizer(&v);
	return (0);
}