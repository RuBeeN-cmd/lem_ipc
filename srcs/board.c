#include "lem_ipc.h"

size_t	get_board_size(void)
{
	return (BOARD_WIDTH * BOARD_HEIGHT * sizeof(uint32_t));
}

size_t	get_board_size_padded(void)
{
	return (PAGE_SIZE * ((get_board_size() + PAGE_SIZE - 1) / PAGE_SIZE));
}

void	init_board(uint32_t **board, uint32_t *raw_board)
{
	for (size_t i = 0; i < BOARD_HEIGHT; i++)
		board[i] = raw_board + i * BOARD_WIDTH;
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
	ft_printf_fd(STDERR_FILENO, "Player %d joined the board at %d %d\n", game->team, game->position.x, game->position.y);
	game->board[game->position.y][game->position.x] = game->team;
	return (0);
}

char	*get_team_color_str(int team)
{
	char	*team_color;
	switch (team)
	{
		case 1:
			team_color = C_BLU;
			break;
		case 2:
			team_color = C_GRN;
			break;
		case 3:
			team_color = C_RED;
			break;
		default:
			team_color = C_WHT;
			break;
	}
	return (team_color);
}

void	print_board(uint32_t **board)
{
	for (size_t y = 0; y < BOARD_HEIGHT; y++)
	{
		for (size_t x = 0; x < BOARD_WIDTH; x++)
		{
			char *color = get_team_color_str(board[y][x]);
			ft_printf_fd(1, "%s%d"C_END, color, board[y][x]);
		}
		ft_printf_fd(1, "\n");
	}
}