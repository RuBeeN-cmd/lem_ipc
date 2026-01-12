#include <player.h>

int	is_two_enemys(t_game *game)
{
	size_t	enemys_id[8];
	int		i = 0;
	for (int y = game->position.y - 1; y <= game->position.y + 1; y++) {
		for (int x = game->position.x - 1; x <= game->position.x + 1; x++) {
			if (x < 0 || x >= game->board_size.x
				|| y < 0 || y >= game->board_size.y)
				continue ;
			else if (game->board[y][x] == EMPTY_CELL)
				continue;
			else if (x == game->position.x && y == game->position.y)
				continue ;
			for (int j = 0; j < i; j++)
				if (enemys_id[j] == game->board[y][x])
					return (enemys_id[j]);
			if (game->board[y][x] != game->team)
				enemys_id[i++] = game->board[y][x];
		}
	}
	return (0);
}

void	move_player(t_game *game, t_vec2 new_pos)
{
	if (new_pos.x >= 0 && new_pos.x < game->board_size.x && new_pos.y >= 0 && new_pos.y < game->board_size.y)
	{
		if (game->board[new_pos.y][new_pos.x] == EMPTY_CELL)
		{
			game->board[game->position.y][game->position.x] = 0;
			game->position = new_pos;
			game->board[new_pos.y][new_pos.x] = game->team;
		}
	}
}

void	move_up(t_game *game)
{
	t_vec2	new_pos;

	new_pos = game->position;
	new_pos.y--;
	move_player(game, new_pos);
}

void	move_down(t_game *game)
{
	t_vec2	new_pos;

	new_pos = game->position;
	new_pos.y++;
	move_player(game, new_pos);
}

void	move_left(t_game *game)
{
	t_vec2	new_pos;

	new_pos = game->position;
	new_pos.x--;
	move_player(game, new_pos);
}

void	move_right(t_game *game)
{
	t_vec2	new_pos;

	new_pos = game->position;
	new_pos.x++;
	move_player(game, new_pos);
}

t_vec2	get_nearest(t_game game, int find_mate)
{
	size_t		max_iter;
	size_t		target_size;
	max_iter = ft_max(game.position.x, game.board_size.x - game.position.x - 1);
	max_iter = ft_max(max_iter, ft_max(game.position.y, game.board_size.y - game.position.y - 1));
	for	(size_t i = 1; i <= max_iter; i++)
	{
		target_size = 2 * i;
		for (size_t j = 0; j < 4; j++)
		{
			t_vec2 target = { game.position.x + ((j >> 1 & 1) * 2 - 1) * i, game.position.y + ((j & 1) * 2 - 1) * i };
			for (size_t k = 0; k < target_size; k++)
			{
				if (target.x >= 0 && target.x < game.board_size.x && target.y >= 0 && target.y < game.board_size.y)
				{
					uint32_t	target_team = game.board[target.y][target.x];
					if (target_team && ((find_mate && target_team == game.team)
						|| (!find_mate && target_team != game.team)))
						return (target);
				}
				if ((j & 1) && (j & 2)) // South
					target.x--;
				else if (j & 1) // West
					target.y--;
				else if (j & 2) // East
					target.y++;
				else // North
					target.x++;
			}
		}
	}
	return ((t_vec2) { -1, -1 });
}

int can_move_straight(t_vec2 target, t_game *game) {
	if (target.x == game->position.x) {
		if (target.y < game->position.y) {
			t_vec2 new_target = {game->position.x, game->position.y - 1};
			if (get_team_on_board(new_target, game->board, game->board_size) == game->team) {
				move_left(game);
				return (0);
			}
		} else if (target.y > game->position.y) {
			t_vec2 new_target = {game->position.x, game->position.y + 1};
			if (get_team_on_board(new_target, game->board, game->board_size) == game->team) {
				move_right(game);
				return (0);
			}
		}
	} else if (target.y == game->position.y) {
		if (target.x < game->position.x) {
			t_vec2 new_target = {game->position.x - 1, game->position.y};
			if (get_team_on_board(new_target, game->board, game->board_size) == game->team) {
				move_up(game);
				return (0);
			}
		} else if (target.x > game->position.x) {
			t_vec2 new_target = {game->position.x + 1, game->position.y};
			if (get_team_on_board(new_target, game->board, game->board_size) == game->team) {
				move_down(game);
				return (0);
			}
		}
	}
	return (1);
}

t_vec2	get_target_pos(t_vec2 enemy, t_game* game) {
	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			if (x == 0 && y == 0)
				continue ;
			t_vec2 target = {enemy.x + x, enemy.y + y};
			if (target.x < 0 || target.x >= game->board_size.x
				|| target.y < 0 || target.y >= game->board_size.y)
				continue;
			if (game->board[target.y][target.x] == EMPTY_CELL)
				return target;
		}
	}
	return (NULL_POS);
}

int	get_best_move(t_game *game)
{
	t_vec2	nearest_enemy = get_nearest(*game, 0);
	if (!vec2cmp(nearest_enemy, NULL_POS))
		return (-1);

	t_vec2 target_pos = get_target_pos(nearest_enemy, game);
	if (!vec2cmp(target_pos, NULL_POS))
		return (-1);

	t_vec2 delta = {target_pos.x - game->position.x, target_pos.y - game->position.y};
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
	return (0);
}

int	is_with_mate(t_game game)
{
	for (int y = game.position.y - 1; y <= game.position.y + 1; y++) {
		if (y < 0 || y >= game.board_size.y)
			continue ;
		for (int x = game.position.x - 1; x <= game.position.x + 1; x++) {
			if ((x < 0 || x >= game.board_size.x) || (x == game.position.x && y == game.position.y))
				continue ;
			else if (game.board[y][x] == game.team)
				return (1);
		}
	}
	return (0);
}

void	go_to_mate(t_game *game)
{
	t_vec2	nearest_mate = get_nearest(*game, 1);
	if (nearest_mate.x == -1 && nearest_mate.y == -1)
		return ;

	t_vec2 delta = {nearest_mate.x - game->position.x, nearest_mate.y - game->position.y};
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

int	is_killed_by_team(t_game *game)
{
	int team_id = is_two_enemys(game);
	if (team_id != 0) {
		game->board[game->position.y][game->position.x] = EMPTY_CELL;
		return (team_id);
	}
	return (0);
}

int	is_other_team(t_game *game, t_ipc *ipc)
{
	sem_lock(ipc->sem_id);
	for (size_t y = 0; y < (size_t) game->board_size.y; y++)
	{
		for (size_t x = 0; x <  (size_t)game->board_size.x; x++)
		{
			if (game->board[y][x] != EMPTY_CELL && game->board[y][x] != game->team)
			{
				sem_unlock(ipc->sem_id);
				return (1);
			}
		}
	}
	sem_unlock(ipc->sem_id);
	return (0);
}

int is_game_paused(t_ipc *ipc)
{
	sem_lock(ipc->sem_id);
	if (check_msg(ipc->msg_id, NULL, 1, PAUSE_CHANNEL) == 1)
	{
		send_msg(ipc->msg_id, "*", 1, PAUSE_CHANNEL);
		return (1);
	}
	sem_unlock(ipc->sem_id);
	return (0);
}

int is_other_mate(t_game game) {
	for (int y = 0; y < game.board_size.y; y++) {
		for (int x = 0; x < game.board_size.x; x++) {
			if (x != game.position.x || y != game.position.y)
				if (game.board[y][x] == game.team)
					return (1);
		}
	}
	return (0);
}

int can_make_move(t_game *game, t_vec2 new_pos) {
	if (new_pos.x >= 0 && new_pos.x < game->board_size.x && new_pos.y >= 0 && new_pos.y < game->board_size.y) {
		if (game->board[new_pos.y][new_pos.x] == EMPTY_CELL)
			return (1);
	}
	return (0);
}

int	can_move_down(t_game *game) {
	t_vec2 new_pos = {game->position.x, game->position.y + 1};
	return (can_make_move(game, new_pos));
}

int can_move_up(t_game *game) {
	t_vec2 new_pos = {game->position.x, game->position.y - 1};
	return (can_make_move(game, new_pos));
}

int can_move_left(t_game *game) {
	t_vec2 new_pos = {game->position.x - 1, game->position.y};
	return (can_make_move(game, new_pos));
}

int can_move_right(t_game *game) {
	t_vec2 new_pos = {game->position.x + 1, game->position.y};
	return (can_make_move(game, new_pos));
}

void escape_from_enemys(t_game *game)
{
	t_vec2	nearest_enemy = get_nearest(*game, 0);
	if (!vec2cmp(nearest_enemy, NULL_POS))
		return ;
	
	t_vec2 delta = {nearest_enemy.x - game->position.x, nearest_enemy.y - game->position.y};
	if (ft_abs(delta.x) > ft_abs(delta.y)) {
		if (delta.x < 0) {
			if (can_move_right(game))
				move_right(game);
			else if (delta.y < 0)
				move_down(game);
			else if (delta.y > 0)
				move_up(game);
		} else {
			if (can_move_left(game))
				move_left(game);
			else if (delta.y < 0)
				move_down(game);
			else if (delta.y > 0)
				move_up(game);
		}
	} else {
		if (delta.y < 0) {
			if (can_move_down(game))
				move_down(game);
			else if (delta.x < 0)
				move_right(game);
			else if (delta.x > 0)
				move_left(game);
		} else {
			if (can_move_up(game))
				move_up(game);
			else if (delta.x < 0)
				move_right(game);
			else if (delta.x > 0)
				move_left(game);
		}
	}
}

void player_routine(t_game *game)
{
	if (!is_other_mate(*game)) {
		escape_from_enemys(game);
	} else if (!is_with_mate(*game)) {
		go_to_mate(game);
	} else {
		get_best_move(game);
	}
}
