#include "lem_ipc.h"

t_vec2	rand_pos(void)
{
	t_vec2 vec2;

	if (ft_rand(&vec2, sizeof(t_vec2)))
		return ((t_vec2) { -1, -1 });
	vec2.x = vec2.x % BOARD_WIDTH;
	if (vec2.x < 0)
		vec2.x = -vec2.x;
	vec2.y = vec2.y % BOARD_HEIGHT;
	if (vec2.y < 0)
		vec2.y = -vec2.y;
	return (vec2);
}

int	join_board(t_game *game)
{
	t_vec2	pos;
	
	pos = rand_pos();
	if (pos.x < 0 || pos.y < 0)
		return (1);
	while (game->board[pos.y][pos.x])
	{
		pos = rand_pos();
		if (pos.x < 0 || pos.y < 0)
			return (1);
	}
	game->position = pos;
	game->board[game->position.y][game->position.x] = game->team;
	return (0);
}

void	init_board(uint32_t **board, uint32_t *raw_board)
{
	for (size_t i = 0; i < BOARD_HEIGHT; i++)
		board[i] = raw_board + i * BOARD_WIDTH;
}

void	init_game(t_game *game, uint32_t *raw_board, uint32_t team)
{
	game->team = team;
	game->visualizer_target = 0;
	init_board(game->board, raw_board);
}
