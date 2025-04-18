#include "lem_ipc.h"

void	free_buffer(uint32_t **buffer, size_t height)
{
	for (size_t i = 0; i < height && buffer[i]; i++)
		free(buffer[i]);
	free(buffer);
}

int	init_buffer(uint32_t ***buffer)
{
	*buffer = (uint32_t **) malloc(BOARD_HEIGHT * sizeof(uint32_t *));
	if (!*buffer)
		return (1);
	for (size_t i = 0; i < BOARD_HEIGHT; i++)
	{
		(*buffer)[i] = (uint32_t *) malloc(BOARD_WIDTH * sizeof(uint32_t *));
		if (!*buffer)
		{
			free_buffer(*buffer, i);
			return (1);
		}
		ft_bzero((*buffer)[i], BOARD_WIDTH * sizeof(uint32_t));
	}
	return (0);
}

void	copy_buffer(uint32_t **dst, uint32_t *src, size_t height)
{
	for (size_t i = 0; i < height; i++) {
		ft_memcpy(dst[i], src, BOARD_WIDTH * sizeof(uint32_t));
		src += BOARD_WIDTH;
	}
}