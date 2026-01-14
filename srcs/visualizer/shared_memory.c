#include <visualizer/visualizer.h>

void	free_board_buffer(uint32_t **buffer, size_t height)
{
	for (size_t i = 0; i < height && buffer[i]; i++)
		free(buffer[i]);
	free(buffer);
}

int	init_board_buffer(uint32_t ***buffer, t_vec2 board_size)
{
	*buffer = (uint32_t **) malloc(board_size.y * sizeof(uint32_t *));
	if (!*buffer)
		return (1);
	for (size_t i = 0; i < (size_t) board_size.y; i++)
	{
		(*buffer)[i] = (uint32_t *) malloc(board_size.x * sizeof(uint32_t *));
		if (!*buffer)
		{
			free_board_buffer(*buffer, i);
			return (1);
		}
		ft_bzero((*buffer)[i], board_size.x * sizeof(uint32_t));
	}
	return (0);
}

void	copy_board(uint32_t **dst, uint32_t *src, t_vec2 board_size)
{
	for (size_t i = 0; i < (size_t) board_size.y; i++) {
		ft_memcpy(dst[i], src, board_size.x * sizeof(uint32_t));
		src += board_size.x;
	}
}

void	try_sync_shm(t_visualizer *v)
{
	if (sem_lock_no_wait(v->ipc.sem_id) != -1)
	{
		copy_board(v->board_copy, (uint32_t *) (v->ipc.data + SHM_BOARD_OFFSET), v->board_size);
		ft_memcpy(&v->shm_copy, v->ipc.data, sizeof(v->ipc.data));
		sem_unlock(v->ipc.sem_id);
	}
}