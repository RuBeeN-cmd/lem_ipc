#include "lem_ipc.h"

void	init_game(t_game *game, uint32_t *raw_board, uint32_t team)
{
	game->team = team;
	init_board(game->board, raw_board);
}

int	is_two_neighboors(t_game *game, t_vec2 target)
{
	for (int y = target.y - 1; y < target.y + 1; y++)
		for (int x = target.x - 1; x < target.x + 1; x++)
			if (x != target.x || y != target.y)
				if (x != game->position.x || y != game->position.y)
					if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT && game->board[y][x] == game->team)
						return (1);
	return (0);
}

void	move_player(t_game *game, t_vec2 new_pos)
{
	if (new_pos.x >= 0 && new_pos.x < BOARD_WIDTH && new_pos.y >= 0 && new_pos.y < BOARD_HEIGHT
		&& (game->board[new_pos.y][new_pos.x] == 0
		|| (game->board[new_pos.y][new_pos.x] != game->team && is_two_neighboors(game, new_pos))))
	{
		game->board[game->position.y][game->position.x] = 0;
		game->position = new_pos;
		game->board[game->position.y][game->position.x] = game->team;
	}
}

void	move_up(t_game *game)
{
	t_vec2	new_pos;

	ft_printf_fd(STDERR_FILENO, "Player %d moving up\n", game->team);
	new_pos = game->position;
	new_pos.y--;
	move_player(game, new_pos);
}

void	move_down(t_game *game)
{
	t_vec2	new_pos;

	ft_printf_fd(STDERR_FILENO, "Player %d moving down\n", game->team);
	new_pos = game->position;
	new_pos.y++;
	move_player(game, new_pos);
}

void	move_left(t_game *game)
{
	t_vec2	new_pos;

	ft_printf_fd(STDERR_FILENO, "Player %d moving left\n", game->team);
	new_pos = game->position;
	new_pos.x--;
	move_player(game, new_pos);
}

void	move_right(t_game *game)
{
	t_vec2	new_pos;

	ft_printf_fd(STDERR_FILENO, "Player %d moving right\n", game->team);
	new_pos = game->position;
	new_pos.x++;
	move_player(game, new_pos);
}

t_vec2	get_nearest_enemy(t_game *game)
{
	size_t		max_iter;
	size_t		target_size;
	max_iter = ft_max(game->position.x, BOARD_WIDTH - game->position.x - 1);
	max_iter = ft_max(max_iter, ft_max(game->position.y, BOARD_HEIGHT - game->position.y - 1));
	for	(size_t i = 1; i <= max_iter; i++)
	{
		target_size = 2 * i;
		for (size_t j = 0; j < 4; j++)
		{
			t_vec2 target;
			target = init_vec2(game->position.x + ((j >> 1 & 1) * 2 - 1) * i, game->position.y + ((j & 1) * 2 - 1) * i);
			for (size_t k = 0; k < target_size; k++)
			{
				if (target.x >= 0 && target.x < BOARD_WIDTH && target.y >= 0 && target.y < BOARD_HEIGHT)
				{
					uint32_t	target_team = game->board[target.y][target.x];
					if (target_team && target_team != game->team)
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
	return (init_vec2(-1, -1));
}

int	get_best_move(t_game *game)
{
	t_vec2	nearest_enemy = get_nearest_enemy(game);
	if (nearest_enemy.x == -1 && nearest_enemy.y == -1)
		return (-1);
	if (nearest_enemy.x < game->position.x)
		move_left(game);
	else if (nearest_enemy.x > game->position.x)
		move_right(game);
	if (nearest_enemy.y < game->position.y)
		move_up(game);
	else if (nearest_enemy.y > game->position.y)
		move_down(game);
	return (0);
}

int	is_alive(t_game game, t_ipc ipc)
{
	sem_lock(ipc.sem_id);
	if (game.board[game.position.y][game.position.x] == game.team)
	{
		sem_unlock(ipc.sem_id);
		return (1);
	}
	sem_unlock(ipc.sem_id);
	return (0);
}

int	is_team_alone(t_game game, t_ipc ipc)
{
	sem_lock(ipc.sem_id);
	for (size_t y = 0; y < BOARD_HEIGHT; y++)
	{
		for (size_t x = 0; x < BOARD_WIDTH; x++)
		{
			if (game.board[y][x] && game.board[y][x] != game.team)
			{
				sem_unlock(ipc.sem_id);
				return (0);
			}
		}
	}
	print_board(game.board);
	sem_unlock(ipc.sem_id);
	return (1);
}

t_vec2	rand_pos(void)
{
	t_vec2 vec2;

	if (ft_rand(&vec2, sizeof(t_vec2)))
		return (init_vec2(-1, -1));
	vec2.x = vec2.x % BOARD_WIDTH;
	if (vec2.x < 0)
		vec2.x = -vec2.x;
	vec2.y = vec2.y % BOARD_HEIGHT;
	if (vec2.y < 0)
		vec2.y = -vec2.y;
	return (vec2);
}