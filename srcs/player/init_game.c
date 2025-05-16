#include <player.h>

t_vec2	rand_pos(t_vec2 board_size)
{
	t_vec2 vec2;

	if (ft_rand(&vec2, sizeof(t_vec2)))
		return ((t_vec2) { -1, -1 });
	vec2.x = vec2.x % board_size.x;
	if (vec2.x < 0)
		vec2.x = -vec2.x;
	vec2.y = vec2.y % board_size.y;
	if (vec2.y < 0)
		vec2.y = -vec2.y;
	return (vec2);
}

int	join_board(t_game *game)
{
	t_vec2	pos;
	
	pos = rand_pos(game->board_size);
	if (pos.x < 0 || pos.y < 0)
		return (1);
	while (game->board[pos.y][pos.x])
	{
		pos = rand_pos(game->board_size);
		if (pos.x < 0 || pos.y < 0)
			return (1);
	}
	game->position = pos;
	game->board[game->position.y][game->position.x] = game->team;
	return (0);
}

void	init_board(uint32_t **board, uint32_t *raw_board, t_vec2 board_size)
{
	for (size_t i = 0; i < board_size.y; i++)
		board[i] = raw_board + i * board_size.x;
}

void	init_game(t_game *game, uint32_t *raw_board, uint32_t team, t_vec2 board_size)
{
	game->team = team;
	game->is_supervised = 0;
	game->board_size = board_size;
	game->board = malloc(sizeof(uint32_t *) * board_size.y);
	init_board(game->board, raw_board, board_size);
}
