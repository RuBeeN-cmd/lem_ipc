#include <player.h>

static void player_loop(t_game *game, t_ipc *ipc)
{
	int killer_team = 0;
	while (1 || is_other_team(game, ipc))
	{
		check_supervision_msg(ipc, game);
		check_team_msg(ipc, game);
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

static t_vec2	is_mate_around(t_game *game, uint32_t radius) {
	for (int local_y = -(int)radius; local_y <= (int) radius; local_y++) {
		int y = game->player.position.y + local_y;
		if (y < 0 || y >= game->board_size.y)
			continue;
		int max_x = radius - ft_abs(local_y);
		for (int local_x = -max_x; local_x <= max_x; local_x++) {
			if (!local_y && !local_x)
				continue;
			int x = game->player.position.x + local_x;
			if (x < 0 || x >= game->board_size.x)
				continue;
			if (game->board[y][x] == game->player.team)
				return (t_vec2) {x, y};
		}
	}
	return NULL_POS;
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

	t_vec2 mate = is_mate_around(&game, SCAN_RADIUS);
	if (!vec2cmp(mate, NULL_POS)) {
		DBG("No mate found around %d moves, leading.\n", SCAN_RADIUS);
		game.player.state = LEADING;
		game.player.chain_id = getpid();
		game.player.last_pos = game.player.position;
		send_team_msg(&ipc, &game);
	}

	#ifdef OSX
		usleep(100000); // for macos: to prevent lock order
	#endif
	player_loop(&game, &ipc);
	close_ipc(&ipc);
	return (0);
}
