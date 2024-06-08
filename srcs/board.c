#include "lem_ipc.h"

size_t	get_board_size(void)
{
	return (BOARD_WIDTH * BOARD_HEIGHT * sizeof(uint32_t));
}

size_t	get_board_size_padded(void)
{
	return (PAGE_SIZE * ((get_board_size() + PAGE_SIZE - 1) / PAGE_SIZE));
}