#include <player.h>

static int	is_team_alive(t_game *game, uint32_t team) {
	for (int y = 0; y < game->board_size.y; y++) {
		for (int x = 0; x < game->board_size.x; x++) {
			if (game->board[y][x] == team) {
				return (1);
			}
		}
	}
	return (0);
}

void	update_targeted_team(t_game *game, t_ipc *ipc) {
	if (!game->player.targeted_team) {
		if (!get_target_team(ipc, game)) {
			choose_target_team(ipc, game);
		}
	} else if (!is_team_alive(game, game->player.targeted_team)) {
		choose_target_team(ipc, game);
	}
}

static void	update_player_target(t_game *game) {
	if (!is_with_mate(game)) {
		game->player.target = get_nearest(game, game->player.team);
	} else if (game->player.targeted_team) {
		game->player.target = get_nearest(game, game->player.targeted_team);
	}
}

static void player_loop(t_game *game, t_ipc *ipc)
{
	int killer_team = 0;
	while (is_other_team(game, ipc))
	{
		check_supervision_msg(ipc, game);
		sem_lock(ipc->sem_id);
		if (!(ipc->data->game_state & PAUSED)) {
			killer_team = is_killed_by_team(game);
			if (killer_team) {
				sem_unlock(ipc->sem_id);
				break ;
			}
			update_targeted_team(game, ipc);
			update_player_target(game);
			player_move(game);
		}
		sem_unlock(ipc->sem_id);
		if (game->player.is_supervised)
			send_supervision_info(ipc, game, 1);
		usleep(COOLDOWN);
	}
	if (killer_team) {
		send_kill_info(ipc, game->player.team, killer_team);
		if (game->player.is_supervised)
			send_supervision_info(ipc, game, 0);
	}
}

int	player_workflow(uint32_t team, t_vec2 board_size)
{
	t_ipc	ipc;
	t_game	game;

	if (init_player_ipc(&ipc, &board_size))
		return (1);
	DBG("IPC initialized\n");
	init_game(&game, ipc.data, team, board_size);
	DBG("Game initialized\n");
	if (ipc_join_board(&ipc, &game))
		return (1);
	DBG("Board joined\n");

	#ifdef OSX
		usleep(100000); // for macos: to prevent lock order
	#endif
	player_loop(&game, &ipc);
	close_ipc(&ipc);
	return (0);
}
