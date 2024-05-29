#include "lem_ipc.h"

int	init_visualizer_semaphore(t_ipc *ipc)
{
	ipc->sem_id = semget(ipc->key, 1, 0);
	if (ipc->sem_id == -1)
		return (1);
	return (0);
}

int	init_visualizer_ipc(t_ipc *ipc)
{
	char	*path = "/Users/rubenrollin/Documents/Ruben/Projects/42/lem_ipc";
	int		proj_id = 'R';
	
	ipc->key = ftok(path, proj_id);
	if (ipc->key == -1)
	{
		perror(path);
		return (1);
	}
	if (init_visualizer_semaphore(ipc) || !(ipc->data = get_shm_data(ipc->key, 0, ipc->sem_id, 0)))
	{
		ft_printf_fd(2, "Nothing to visualize\n");
		return (1);
	}
	return (0);
}