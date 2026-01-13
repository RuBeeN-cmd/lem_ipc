#include <player.h>

static void player_loop(t_game *game, t_ipc *ipc)
{
	int killer_team = 0;
	while (is_other_team(game, ipc))
	{
		check_supervision_msg(ipc, game);
		if (get_game_state(ipc->sem_id, &(ipc->data->game_state)) != PAUSED) {
			sem_lock(ipc->sem_id);
			killer_team = is_killed_by_team(game);
			if (killer_team) {
				sem_unlock(ipc->sem_id);
				break ;
			}
			player_routine(game);
			sem_unlock(ipc->sem_id);
		}
		if (game->is_supervised) {
			DBG("Player [%d] is sending info...\n", game->team);
			send_supervision_info(ipc, game, 1);
		}
		usleep(COOLDOWN);
	}
	if (killer_team) {
		send_kill_info(ipc, game->team, killer_team);
		if (game->is_supervised)
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
