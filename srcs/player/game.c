#include <player.h>

int	is_two_enemys(t_game *game)
{
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

int	is_killed_by_team(t_game *game)
{
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

t_vec2	get_closest(t_game *game, t_vec2 target) {
	int	max_radius = ft_max(
		ft_max(target.x, game->board_size.x - target.x - 1),
		ft_max(target.y, game->board_size.y - target.y - 1)
	);
	for (int radius = 1; radius <= max_radius; radius++) {
		uint32_t	min_dist = UINT32_MAX;
		t_vec2		min_pos = NULL_POS;
		for (int dir = 0; dir < 4; dir++) {			// NORTH, WEST, SOUTH, EAST
			t_vec2 cell = add_vec2(target, (t_vec2) {
				radius * (dir & 1) * ((dir & 2) - 1),
				radius * (!(dir & 1)) * ((dir & 2) - 1)
			});
			t_vec2 increment = (t_vec2) {
				1 - (dir & 2),						// SOUTH, EAST: x--		NORTH, WEST: x++ 
				1 - (((dir & 1) << 1) ^ (dir & 2))	// WEST, SOUTH: y-- 	NORTH, EAST: y++ 
			};
			for (int i = 0; i < radius; i++) {
				if (cell.x < 0 || cell.x >= game->board_size.x || cell.y < 0 || cell.y >= game->board_size.y)
					continue ;
				if (!game->board[cell.y][cell.x]) {
					t_vec2 delta = sub_vec2(game->player.position, cell);
					uint32_t dist = ft_abs(delta.x) + ft_abs(delta.y);
					if (dist < min_dist) {
						min_pos = cell;	
						min_dist = dist;
					}

				}
				cell = add_vec2(cell, increment);
			}
		}
		if (vec2cmp(min_pos, NULL_POS))
			return (min_pos);
	}
	return (NULL_POS);
}

int	can_move_player(t_game *game, t_vec2 new_pos)
{
	if (new_pos.x >= 0 && new_pos.x < game->board_size.x && new_pos.y >= 0 && new_pos.y < game->board_size.y)
	{
		if (game->board[new_pos.y][new_pos.x] == EMPTY_CELL)
		{
			return (1);
		}
	}
	return (0);
}

int	move_player(t_game *game, t_vec2 new_pos)
{
	if (can_move_player(game, new_pos))
	{
		game->board[game->player.position.y][game->player.position.x] = 0;
		game->player.position = new_pos;
		game->board[new_pos.y][new_pos.x] = game->player.team;
		return (1);
	}
	return (0);
}

int	move_up(t_game *game) {
	t_vec2	new_pos = game->player.position;
	new_pos.y--;
	return move_player(game, new_pos);
}

int	move_down(t_game *game) {
	t_vec2	new_pos = game->player.position;
	new_pos.y++;
	return move_player(game, new_pos);
}

int	move_left(t_game *game) {
	t_vec2	new_pos = game->player.position;
	new_pos.x--;
	return move_player(game, new_pos);
}

int	move_right(t_game *game) {
	t_vec2	new_pos = game->player.position;
	new_pos.x++;
	return move_player(game, new_pos);
}

// int player_move(t_game *game) {
// 	if (!game->player.is_leader)
// 		return go_to_leader(game);
// 	// else
// 	// 	return get_best_move(game);
// 	return (0);
// 	// if (!is_other_mate(*game)) {
// 	// 	escape_from_enemys(game);
// 	// } else if (!is_with_mate(*game)) {
// 	// 	go_to_mate(game);
// 	// } else {
// 	// 	get_best_move(game);
// 	// }
// }
