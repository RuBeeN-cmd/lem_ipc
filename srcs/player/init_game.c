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

int	is_mate_in_chunk(t_game *game, t_vec2 pos) {
	t_vec2	chunk_pos = (t_vec2) { pos.x / game->chunk_size, pos.y / game->chunk_size };
	for (uint32_t local_y = 0; local_y < game->chunk_size; local_y++) {
		uint32_t y = chunk_pos.y * game->chunk_size + local_y;
		if (y < 0 || y >= (uint32_t) game->board_size.y)
			continue;
		for (uint32_t local_x = 0; local_x < game->chunk_size; local_x++) {
			uint32_t x = chunk_pos.x * game->chunk_size + local_x;
			if (x < 0 || x >= (uint32_t) game->board_size.x)
				continue;
			if (x == (uint32_t) pos.x && y == (uint32_t) pos.y)
				continue;
			if (game->board[y][x] == game->player.team)
				return (1);
		}
	}
	return (0);
}

int	join_board(t_game *game)
{
	t_vec2	pos;
	
	pos = rand_pos(game->board_size);
	if (pos.x < 0 || pos.y < 0)
		return (1);
	t_vec2	start_pos = pos;
	while (game->board[pos.y][pos.x]) {
		pos.x++;
		if (pos.x >= game->board_size.x) {
			pos.x = 0;
			pos.y++;
		}
		if (pos.y >= game->board_size.y)
			pos.y = 0;
		if (pos.x == start_pos.x && pos.y == start_pos.y)
			return (1);
	}
	// if (!is_mate_in_chunk(game, pos))
	if (!is_other_mate(*game)) {
		DBG("Defined as leader !\n");
		game->player.is_leader = 1;
	}
	game->player.position = pos;
	game->board[game->player.position.y][game->player.position.x] = game->player.team;
	return (0);
}

void	init_board(uint32_t **board, uint32_t *raw_board, t_vec2 board_size)
{
	for (size_t i = 0; i < (size_t) board_size.y; i++)
		board[i] = raw_board + i * board_size.x;
}

void	init_game(t_game *game, t_shm_data *shm_data, uint32_t team, t_vec2 board_size)
{
	init_player(&game->player, team);
	game->chunk_size = 10;
	game->board_size = board_size;
	game->board = malloc(sizeof(uint32_t *) * board_size.y);
	init_board(game->board, (uint32_t *) (shm_data + SHM_BOARD_OFFSET), board_size);
}
