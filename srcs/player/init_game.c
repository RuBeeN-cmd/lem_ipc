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

// static t_vec2	is_mate_around(t_game *game, uint32_t radius) {
// 	for (int local_y = -(int)radius; local_y <= (int) radius; local_y++) {
// 		int y = game->player.position.y + local_y;
// 		if (y < 0 || y >= game->board_size.y)
// 			continue;
// 		int max_x = radius - ft_abs(local_y);
// 		for (int local_x = -max_x; local_x <= max_x; local_x++) {
// 			if (!local_y && !local_x)
// 				continue;
// 			int x = game->player.position.x + local_x;
// 			if (x < 0 || x >= game->board_size.x)
// 				continue;
// 			if (game->board[y][x] == game->player.team)
// 				return (t_vec2) {x, y};
// 		}
// 	}
// 	return NULL_POS;
// }

static int	is_first(t_game *game) {
	for (int y = 0; y < game->board_size.y; y++) {
		for (int x = 0; x < game->board_size.x; x++) {
			if (x == game->player.position.x && y == game->player.position.y)
				continue;
			if (game->board[y][x] == game->player.team) {
				return (0);
			}			
		}
	}
	return (1);
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
	game->player.position = pos;
	game->board[game->player.position.y][game->player.position.x] = game->player.team;
	INFO("Board joined, position: (%d, %d)\n", pos.x, pos.y);
	
	if (is_first(game)) {
		game->player.chain_id = getpid();
		DBG("No mate found around %d moves, chain %d created.\n", SCAN_RADIUS, game->player.chain_id);
	}
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
