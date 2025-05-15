#include <player.h>

static void player_routine(t_game *game)
{
	if (!is_with_mate(*game)) {
		go_to_mate(game);
	} else {
		get_best_move(game);
	}
}

static void	check_supervision_msg(t_ipc *ipc, t_game *game)
{
	t_new_target_msg visualizer_target;
	int ret = check_msg(ipc->msg_id, &visualizer_target, sizeof(visualizer_target), VISUALIZER_TARGET_CHANNEL);
	if (ret == 1)
	{
		if (!vec2cmp(visualizer_target.target, game->position)) {
			if (visualizer_target.type == STOP_TARGETING)
				ft_log(LOG_DEBUG, "Player catch: STOP TARGET\n");
			else
				ft_log(LOG_DEBUG, "Player catch: NEW TARGET\n");
			game->is_supervised = visualizer_target.type;
		}
		else
			send_msg(ipc->msg_id, &visualizer_target, sizeof(visualizer_target), VISUALIZER_TARGET_CHANNEL);
	}
}

static void	send_supervision_info(t_ipc *ipc, t_game *game, int is_alive)
{
	t_supervised_infos target_infos = {
		game->position,
		game->team,
		is_alive
	};
	send_msg(ipc->msg_id, &target_infos, sizeof(t_supervised_infos), TARGET_INFOS_CHANNEL);
}

static void player_loop(t_game *game, t_ipc *ipc)
{
	int is_player_alive = 1;
	while ((is_player_alive = is_alive(game, ipc)) && is_other_team(game, ipc))
	{
		sem_lock(ipc->sem_id);
		check_supervision_msg(ipc, game);
		if (!is_game_paused(ipc))
			player_routine(game);
		if (game->is_supervised) {
			ft_printf_fd(1, "Player [%d] is sending info...\n", game->team);
			send_supervision_info(ipc, game, 1);
		}
		sem_unlock(ipc->sem_id);
		usleep(COOLDOWN);
	}
	if (!is_player_alive && game->is_supervised)
	{
		sem_lock(ipc->sem_id);
		send_supervision_info(ipc, game, is_player_alive);
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
