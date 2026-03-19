#include <player.h>

int	is_two_enemys(t_game *game) {
	size_t	enemys_id[8];
	int		i = 0;
	for (int y = game->player.position.y - 1; y <= game->player.position.y + 1; y++) {
		for (int x = game->player.position.x - 1; x <= game->player.position.x + 1; x++) {
			if (x < 0 || x >= game->board_size.x
				|| y < 0 || y >= game->board_size.y)
				continue ;
			else if (game->board[y][x] == EMPTY_CELL)
				continue;
			else if (x == game->player.position.x && y == game->player.position.y)
				continue ;
			for (int j = 0; j < i; j++)
				if (enemys_id[j] == game->board[y][x])
					return (enemys_id[j]);
			if (game->board[y][x] != game->player.team)
				enemys_id[i++] = game->board[y][x];
		}
	}
	return (0);
}

int	is_killed_by_team(t_game *game) {
	int team_id = is_two_enemys(game);
	if (team_id != 0) {
		game->board[game->player.position.y][game->player.position.x] = EMPTY_CELL;
		return (team_id);
	}
	return (0);
}

int is_other_mate(t_game game) {
	for (int y = 0; y < game.board_size.y; y++) {
		for (int x = 0; x < game.board_size.x; x++) {
			if (x != game.player.position.x || y != game.player.position.y)
				if (game.board[y][x] == game.player.team)
					return (1);
		}
	}
	return (0);
}

int is_other_team(t_game *game, t_ipc *ipc) {
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