#include "lem_ipc.h"

void	init_game(t_game *game, int *raw_board, int team)
{
	game->team = team;
	init_board(game->board, raw_board);
}

void	move_player(t_game *game, t_vec2 new_pos)
{
	if (new_pos.x >= 0 && new_pos.x < BOARD_WIDTH && new_pos.y >= 0 && new_pos.y < BOARD_HEIGHT && !game->board[new_pos.y][new_pos.x])
	{
		game->board[game->position.y][game->position.x] = 0;
		game->position = new_pos;
		game->board[game->position.y][game->position.x] = game->team;
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

void	move_random(t_game *game)
{
	int		rand_move;

	rand_move = rand() % 4;
	if (rand_move == 0)
		move_up(game);
	else if (rand_move == 1)
		move_down(game);
	else if (rand_move == 2)
		move_left(game);
	else
		move_right(game);
}