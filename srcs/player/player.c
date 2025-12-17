#include <player.h>

static void player_loop(t_game *game, t_ipc *ipc)
{
	int killer_team = 0;
	while (!(killer_team = is_killed_by_team(game, ipc)) && is_other_team(game, ipc))
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
	if (killer_team)
	{
		sem_lock(ipc->sem_id);
		send_kill_info(ipc, killer_team, game->team);
		if (game->is_supervised)
			send_supervision_info(ipc, game, 0);
		sem_unlock(ipc->sem_id);
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
	#ifdef OSX
		usleep(100000); // for macos: to prevent lock order
	#endif
	player_loop(&game, &ipc);
	close_ipc(&ipc);
	return (0);
}
