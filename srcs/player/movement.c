#include <player.h>

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

// int	can_move_player(t_game *game, t_vec2 new_pos)
// {
// 	if (new_pos.x >= 0 && new_pos.x < game->board_size.x && new_pos.y >= 0 && new_pos.y < game->board_size.y)
// 	{
// 		if (game->board[new_pos.y][new_pos.x] == EMPTY_CELL)
// 		{
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

// int	move_player(t_game *game, t_vec2 new_pos)
// {
// 	if (can_move_player(game, new_pos))
// 	{
// 		game->board[game->player.position.y][game->player.position.x] = 0;
// 		game->player.position = new_pos;
// 		game->board[new_pos.y][new_pos.x] = game->player.team;
// 		return (1);
// 	}
// 	return (0);
// }

// int	move_up(t_game *game) {
// 	t_vec2	new_pos = game->player.position;
// 	new_pos.y--;
// 	return move_player(game, new_pos);
// }

// int	move_down(t_game *game) {
// 	t_vec2	new_pos = game->player.position;
// 	new_pos.y++;
// 	return move_player(game, new_pos);
// }

// int	move_left(t_game *game) {
// 	t_vec2	new_pos = game->player.position;
// 	new_pos.x--;
// 	return move_player(game, new_pos);
// }

// int	move_right(t_game *game) {
// 	t_vec2	new_pos = game->player.position;
// 	new_pos.x++;
// 	return move_player(game, new_pos);
// }

t_vec2	get_nearest(t_game *game, int find_mate)
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
					if (target_team && ((find_mate && target_team == game->player.team)
						|| (!find_mate && target_team != game->player.team)))
						return (target);
				}
				target = add_vec2(target, increment);
			}
		}
	}
	return (NULL_POS);
}

// int can_move_straight(t_vec2 target, t_game *game) {
// 	if (target.x == game->player.position.x) {
// 		if (target.y < game->player.position.y) {
// 			t_vec2 new_target = {game->player.position.x, game->player.position.y - 1};
// 			if (get_team_on_board(new_target, game->board, game->board_size) == game->player.team) {
// 				move_left(game);
// 				return (0);
// 			}
// 		} else if (target.y > game->player.position.y) {
// 			t_vec2 new_target = {game->player.position.x, game->player.position.y + 1};
// 			if (get_team_on_board(new_target, game->board, game->board_size) == game->player.team) {
// 				move_right(game);
// 				return (0);
// 			}
// 		}
// 	} else if (target.y == game->player.position.y) {
// 		if (target.x < game->player.position.x) {
// 			t_vec2 new_target = {game->player.position.x - 1, game->player.position.y};
// 			if (get_team_on_board(new_target, game->board, game->board_size) == game->player.team) {
// 				move_up(game);
// 				return (0);
// 			}
// 		} else if (target.x > game->player.position.x) {
// 			t_vec2 new_target = {game->player.position.x + 1, game->player.position.y};
// 			if (get_team_on_board(new_target, game->board, game->board_size) == game->player.team) {
// 				move_down(game);
// 				return (0);
// 			}
// 		}
// 	}
// 	return (1);
// }

t_vec2	get_target_pos(t_vec2 enemy, t_game* game) {
	int		min_dist = 0;
	t_vec2	min_pos = NULL_POS; 
	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			if (x == 0 && y == 0)
				continue ;
			t_vec2 target = {enemy.x + x, enemy.y + y};
			if (target.x < 0 || target.x >= game->board_size.x
				|| target.y < 0 || target.y >= game->board_size.y)
				continue;
			if (game->board[target.y][target.x] == EMPTY_CELL) {
				t_vec2 delta = sub_vec2(enemy, game->player.position);
				int dist = ft_abs(delta.x) + ft_abs(delta.y);
				if (min_dist == 0 || dist < min_dist) {
					min_dist = dist;
					min_pos = target;
				}
			}
		}
	}
	return (min_pos);
}

int	get_best_move(t_game *game)
{
	t_vec2	nearest_enemy = get_nearest(game, 0);
	if (!vec2cmp(nearest_enemy, NULL_POS))
		return (0);

	t_vec2 target_pos = get_target_pos(nearest_enemy, game);
	if (!vec2cmp(target_pos, NULL_POS))
		return (0);

	t_vec2 delta = {target_pos.x - game->player.position.x, target_pos.y - game->player.position.y};
	if (ft_abs(delta.x) > ft_abs(delta.y)) {
		if (delta.x > 0)
			return (move_right(game));
		else
			return (move_left(game));
	} else {
		if (delta.y > 0)
			return (move_down(game));
		else
			return (move_up(game));
	}
	return (0);
}

int	is_with_mate(t_game *game)
{
	for (int y = game->player.position.y - 1; y <= game->player.position.y + 1; y++) {
		if (y < 0 || y >= game->board_size.y)
			continue ;
		for (int x = game->player.position.x - 1; x <= game->player.position.x + 1; x++) {
			if ((x < 0 || x >= game->board_size.x) || (x == game->player.position.x && y == game->player.position.y))
				continue ;
			else if (game->board[y][x] == game->player.team)
				return (1);
		}
	}
	return (0);
}

void	go_to_mate(t_game *game)
{
	t_vec2	nearest_mate = get_nearest(game, 1);
	if (nearest_mate.x == -1 && nearest_mate.y == -1)
		return ;

	t_vec2 delta = {nearest_mate.x - game->player.position.x, nearest_mate.y - game->player.position.y};
	if (ft_abs(delta.x) > ft_abs(delta.y)) {
		if (delta.x > 0)
			move_right(game);
		else
			move_left(game);
	} else {
		if (delta.y > 0)
			move_down(game);
		else
			move_up(game);
	}
}

// int	is_other_team(t_game *game, t_ipc *ipc)
// {
// 	sem_lock(ipc->sem_id);
// 	for (size_t y = 0; y < (size_t) game->board_size.y; y++)
// 	{
// 		for (size_t x = 0; x <  (size_t)game->board_size.x; x++)
// 		{
// 			if (game->board[y][x] != EMPTY_CELL && game->board[y][x] != game->player.team)
// 			{
// 				sem_unlock(ipc->sem_id);
// 				return (1);
// 			}
// 		}
// 	}
// 	sem_unlock(ipc->sem_id);
// 	return (0);
// }

// int can_make_move(t_game *game, t_vec2 new_pos) {
// 	if (new_pos.x >= 0 && new_pos.x < game->board_size.x && new_pos.y >= 0 && new_pos.y < game->board_size.y) {
// 		if (game->board[new_pos.y][new_pos.x] == EMPTY_CELL)
// 			return (1);
// 	}
// 	return (0);
// }

// int	can_move_down(t_game *game) {
// 	t_vec2 new_pos = {game->player.position.x, game->player.position.y + 1};
// 	return (can_make_move(game, new_pos));
// }

// int can_move_up(t_game *game) {
// 	t_vec2 new_pos = {game->player.position.x, game->player.position.y - 1};
// 	return (can_make_move(game, new_pos));
// }

// int can_move_left(t_game *game) {
// 	t_vec2 new_pos = {game->player.position.x - 1, game->player.position.y};
// 	return (can_make_move(game, new_pos));
// }

// int can_move_right(t_game *game) {
// 	t_vec2 new_pos = {game->player.position.x + 1, game->player.position.y};
// 	return (can_make_move(game, new_pos));
// }

void escape_from_enemys(t_game *game)
{
	t_vec2	nearest_enemy = get_nearest(game, 0);
	if (!vec2cmp(nearest_enemy, NULL_POS))
		return ;
	
	t_vec2 delta = {nearest_enemy.x - game->player.position.x, nearest_enemy.y - game->player.position.y};
	if (ft_abs(delta.x) > ft_abs(delta.y)) {
		if (delta.x < 0) {
			move_right(game);
		} else {
			move_left(game);
		}
	} else {
		if (delta.y < 0) {
			move_down(game);
		} else {
			move_up(game);
		}
	}
}

int player_move(t_game *game) {
	if (!is_other_mate(*game)) {
		escape_from_enemys(game);
	} else if (!is_with_mate(game)) {
		go_to_mate(game);
	} else {
		get_best_move(game);
	}
	return (0);
}