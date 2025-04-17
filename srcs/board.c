#include "lem_ipc.h"

uint32_t	get_target_team(t_vec2 target, uint32_t *board[]) {
	if (target.x < 0 || target.x >= BOARD_WIDTH
		|| target.y < 0 || target.y >= BOARD_HEIGHT)
		return (VISUALIZER_CHANNEL); // Error case
	else
		return (board[target.y][target.x]);
}

size_t	get_board_size(void)
{
	return (BOARD_WIDTH * BOARD_HEIGHT * sizeof(uint32_t));
}

size_t	get_board_size_padded(void)
{
	return (PAGE_SIZE * ((get_board_size() + PAGE_SIZE - 1) / PAGE_SIZE));
}