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

t_vec2	get_nearest(t_game *game, uint32_t to_find)
{
	int dist_max = ft_max(game->player.position.x, game->board_size.x - game->player.position.x - 1)
		+ ft_max(game->player.position.y, game->board_size.y - game->player.position.y - 1);
	for (int dist = 1; dist <= dist_max; dist++) {
		for (int dir = 0; dir < 4; dir++) {			// NORTH, WEST, SOUTH, EAST
			t_vec2 target = add_vec2(game->player.position, (t_vec2) {
				dist * (dir & 1) * ((dir & 2) - 1),
				dist * (!(dir & 1)) * ((dir & 2) - 1)
			});
			t_vec2 increment = (t_vec2) {
				1 - (dir & 2),						// SOUTH, EAST: x--		NORTH, WEST: x++ 
				1 - (((dir & 1) << 1) ^ (dir & 2))	// WEST, SOUTH: y-- 	NORTH, EAST: y++ 
			};
			for (int i = 0; i < dist; i++) {
				if (target.x >= 0 && target.x < game->board_size.x && target.y >= 0 && target.y < game->board_size.y) {
					uint32_t target_team = game->board[target.y][target.x];
					if (target_team && ((target_team == to_find)
						|| (!to_find && target_team != game->player.team)))
						return (target);
				}
				target = add_vec2(target, increment);
			}
		}
	}
	return (NULL_POS);
}

int	is_with_mate(t_game *game)
{
	for (int y = game->player.position.y - 2; y <= game->player.position.y + 2; y++) {
		if (y < 0 || y >= game->board_size.y)
			continue ;
		for (int x = game->player.position.x - 2; x <= game->player.position.x + 2; x++) {
			if ((x < 0 || x >= game->board_size.x) || (x == game->player.position.x && y == game->player.position.y))
				continue ;
			else if (game->board[y][x] == game->player.team)
				return (1);
		}
	}
	return (0);
}