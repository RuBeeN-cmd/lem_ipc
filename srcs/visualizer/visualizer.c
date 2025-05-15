#include <visualizer/visualizer.h>

static int	init_visualizer(t_visualizer *v, char title[], uint32_t width, uint32_t height)
{
	if (init_visualizer_ipc(&v->ipc, BOARD_SIZE)) {
		ft_printf_fd(2, "Nothing to visualize\n");
		return (1);
	}
	if (init_buffer(&v->buffer))
		return (1);
	send_msg(v->ipc.msg_id, "*", 1, VISUALIZER_CHANNEL);
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
	v->supervision_panel = init_panel(
		(t_vec2) {SUPERVISION_PANEL_WIDTH, SUPERVISION_PANEL_HEIGHT},
		ANCHOR_TOP_RIGHT
	);
	v->supervision_panel.visible = 0;
	if (init_sdl(v, title, width, height))
	{
		free_buffer(v->buffer, BOARD_HEIGHT);
		return (1);
	}
	return (0);
}

static void	destroy_visualizer(t_visualizer *v)
{
	check_msg(v->ipc.msg_id, NULL, 1, VISUALIZER_CHANNEL);
	destroy_text_line_list(&v->supervision_panel);
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

static void	create_panel_text_lines(t_visualizer *v, t_panel *panel, t_supervised_infos target_infos)
{
	if (panel->text_line_list)
		destroy_text_line_list(panel);
	panel->text_line_list = NULL;
	add_text_line(v->text_engine, v->font, panel, "Target Info : ", color_from_u32(0xFF000000), JUSTIFY_CENTER, 0);

	add_text_line(v->text_engine, v->font, panel, "Position : ", color_from_u32(0xFF000000), JUSTIFY_LEFT, 1);
	char *str = "(";
	str = ft_strjoin_free(str, ft_itoa(target_infos.pos.x), 2);
	str = ft_strjoin_free(str, ", ", 1);
	str = ft_strjoin_free(str, ft_itoa(target_infos.pos.y), 3);
	str = ft_strjoin_free(str, ")", 1);
	add_text_line(v->text_engine, v->font, panel, str, color_from_u32(0xFF000000), JUSTIFY_RIGHT, 1);
	free(str);

	add_text_line(v->text_engine, v->font, panel, "Team : ", color_from_u32(0xFF000000), JUSTIFY_LEFT, 2);
	str = ft_itoa(target_infos.team);
	add_text_line(v->text_engine, v->font, panel, str, get_team_color(target_infos.team), JUSTIFY_RIGHT, 2);
	free(str);

	add_text_line(v->text_engine, v->font, panel, "Alive : ", color_from_u32(0xFF000000), JUSTIFY_LEFT, 3);
	str = target_infos.is_alive ? "Yes" : "No";
	t_color alive_color = target_infos.is_alive ? color_from_u32(0xFF00FF00) : color_from_u32(0xFF0000FF);
	add_text_line(v->text_engine, v->font, panel, str, alive_color, JUSTIFY_RIGHT, 3);
}

static int	visualizer_routine(t_visualizer *v)
{
	if (handle_events(v))
		return (0);
	if (sem_lock_no_wait(v->ipc.sem_id) != -1)
	{
		if (v->running == -1)
		{
			if (check_msg(v->ipc.msg_id, NULL, 1, PAUSE_CHANNEL) == 1)
			{
				send_msg(v->ipc.msg_id, "*", 1, PAUSE_CHANNEL);
				v->running = 0;
			}
			else
				v->running = 1;
		}
		check_msg(v->ipc.msg_id, &v->target_infos, sizeof(v->target_infos), TARGET_INFOS_CHANNEL);
		create_panel_text_lines(v, &v->supervision_panel, v->target_infos);
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