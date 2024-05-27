#include "lem_ipc.h"

size_t	get_board_size(void)
{
	return (BOARD_WIDTH * BOARD_HEIGHT * sizeof(int));
}

size_t	get_board_size_padded(void)
{
	return (PAGE_SIZE * ((get_board_size() + PAGE_SIZE - 1) / PAGE_SIZE));
}

void	init_board(int **board, int *raw_board)
{
	for (size_t i = 0; i < BOARD_HEIGHT; i++)
		board[i] = raw_board + i * BOARD_WIDTH;
}

void	show_board(int **board)
{
	for (size_t i = 0; i < BOARD_HEIGHT; i++)
	{
		for (size_t j = 0; j < BOARD_WIDTH; j++)
		{
			char	*color;
			if (!board[i][j])
				ft_printf_fd(1, ".");
			else if (board[i][j] == 1)
				color = C_BLU;
			else if (board[i][j] == 2)
				color = C_RED;
			else if (board[i][j] == 3)
				color = C_YEL;
			else if (board[i][j] == 4)
				color = C_MAG;
			else if (board[i][j] == 5)
				color = C_GRN;
			else if (board[i][j] == 6)
				color = C_CYN;
			else
				color = C_WHT;
			if (board[i][j])
				ft_printf_fd(1, "%s%d"C_END, color, board[i][j]);
		}
		ft_printf_fd(1, "\n");
	}
}

void	join_board(t_game *game)
{
	t_vec2	pos;

	pos.x = rand() % BOARD_WIDTH;
	pos.y = rand() % BOARD_HEIGHT;
	while (game->board[pos.y][pos.x])
	{
		pos.x = rand() % BOARD_WIDTH;
		pos.y = rand() % BOARD_HEIGHT;
	}
	game->position = pos;
	game->board[game->position.y][game->position.x] = game->team;
}