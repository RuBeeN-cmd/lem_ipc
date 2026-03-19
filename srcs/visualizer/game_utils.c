#include <visualizer/visualizer.h>

int	is_game_ended(uint32_t **board, t_vec2 board_size)
{
	uint32_t found_id = 0;
	for (int y = 0; y < board_size.y; y++) {
		for (int x = 0; x < board_size.x; x++) {
			if (board[y][x] != EMPTY_CELL) {
				if (found_id == 0)
					found_id = board[y][x];
				else if (board[y][x] != found_id)
					return (1);
			}
		}
	}
	return (found_id == 0);
}