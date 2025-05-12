#include <player.h>

static void player_routine(t_game *game)
{
	if (!is_with_mate(*game)) {
		go_to_mate(game);
	} else {
		get_best_move(game);
	}
}

static void player_loop(t_game *game, t_ipc *ipc)
{
	int is_player_alive = 1;
	while ((is_player_alive = is_alive(game, ipc)) && is_other_team(game, ipc))
	{
		sem_lock(ipc->sem_id);
		t_vec2 visualizer_target;
		int ret = check_visualizer_target_msg(ipc->msg_id, &visualizer_target);
		ft_printf_fd(1, "Ret: %d\n", ret);
		if (ret == 1)
		{
			ft_printf_fd(1, "Visualizer Target: (%d,%d)\n", visualizer_target.x, visualizer_target.y);
			if (!vec2cmp(visualizer_target, game->position))
				game->is_supervised = 1;
			else {
				// if (game->is_supervised == 1)
				// game->is_supervised = 0;
				send_visualizer_target_msg(ipc->msg_id, visualizer_target);
			}
		}
		if (!is_game_paused(ipc))
			player_routine(game);
		if (game->is_supervised) {
			t_supervised_infos target_infos = {
				game->position,
				game->team,
				is_player_alive
			};
			send_target_infos_msg(ipc->msg_id, target_infos);
		}
		sem_unlock(ipc->sem_id);
		usleep(COOLDOWN);
	}
	if (!is_player_alive && game->is_supervised)
	{
		sem_lock(ipc->sem_id);
		t_supervised_infos target_infos = {
			game->position,
			game->team,
			is_player_alive
		};
		send_target_infos_msg(ipc->msg_id, target_infos);
		sem_unlock(ipc->sem_id);
	}
}

int	player_workflow(uint32_t team)
{
	t_ipc	ipc;
	t_game	game;

	if (init_player_ipc(&ipc, BOARD_SIZE))
		return (1);
	init_game(&game, ipc.data, team);
	if (ipc_join_board(&ipc, &game))
		return (1);
	#ifdef OSX
		usleep(100000); // for macos: to prevent lock order
	#endif
	player_loop(&game, &ipc);
	close_ipc(&ipc);
	return (0);
}
