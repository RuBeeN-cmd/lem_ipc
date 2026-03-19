#include <player.h>

static int is_other_team(t_game *game, t_ipc *ipc)
{
	sem_lock(ipc->sem_id);
	for (size_t y = 0; y < (size_t) game->board_size.y; y++)
	{
		for (size_t x = 0; x <  (size_t)game->board_size.x; x++)
		{
			if (game->board[y][x] != EMPTY_CELL && game->board[y][x] != game->player.team)
			{
				sem_unlock(ipc->sem_id);
				return (1);
			}
		}
	}
	sem_unlock(ipc->sem_id);
	return (0);
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
