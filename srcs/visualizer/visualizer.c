#include <visualizer.h>

static int	init_visualizer(t_visualizer *v, char title[], uint32_t width, uint32_t height)
{
	if (init_visualizer_ipc(&v->ipc, BOARD_SIZE)) {
		ft_printf_fd(2, "Nothing to visualize\n");
		return (1);
    }
	if (init_buffer(&v->buffer))
		return (1);
		
	v->running = -1;
	v->cell_size = (height - INITIAL_PADDING) / BOARD_HEIGHT;
	if (v->cell_size * BOARD_WIDTH > (width - INITIAL_PADDING))
		v->cell_size = (width - INITIAL_PADDING) / BOARD_WIDTH;
	v->offset = (t_fvec2) {0, 0};
	v->target_infos = (t_supervised_infos) {
		.pos = NULL_POS,
		.team = -1,
		.is_alive = 0
	};
	if (init_sdl(v, title, width, height))
	{
		free_buffer(v->buffer, BOARD_HEIGHT);
		return (1);
	}
	return (0);
}

static void	destroy_visualizer(t_visualizer *v)
{
	destroy_sdl(v);
	free_buffer(v->buffer, BOARD_HEIGHT);
	exit(0);
}

static int	is_game_ended(uint32_t **board)
{
	uint32_t found_id = 0;
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (board[y][x] != EMPTY_CELL) {
				if (found_id == 0)
					found_id = board[y][x];
				else if (board[y][x] != found_id)
					return (1);
			}
		}
	}
	return (found_id == 0);
}

static int	visualizer_routine(t_visualizer *v)
{
	if (handle_events(v))
		return (0);
	if (sem_lock_no_wait(v->ipc.sem_id) != -1)
	{
		if (v->running == -1)
		{
			if (check_pause_msg(v->ipc.msg_id) == 1)
			{
				send_pause_msg(v->ipc.msg_id);
				v->running = 0;
			}
			else
				v->running = 1;
		}
		check_target_infos_msg(v->ipc.msg_id, &v->target_infos);
		ft_printf_fd(1, "Visualizer Target: (%d,%d)\n", v->target_infos.pos.x, v->target_infos.pos.y);
		copy_buffer(v->buffer, v->ipc.data, BOARD_HEIGHT);
		sem_unlock(v->ipc.sem_id);
	}
	clear_window(v->renderer, color_from_u32(0xFF000000));
	draw_board(v);
	draw_target_infos(v, NULL);
	SDL_RenderPresent(v->renderer);
	if (!is_game_ended(v->buffer))
		return (0);
	return (1);
}

static void	visualizer_loop(t_visualizer *v)
{
	while (visualizer_routine(v))
		SDL_Delay(16);
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