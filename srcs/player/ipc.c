#include <player.h>

int	ipc_join_board(t_ipc *ipc, t_game *game)
{
	if (ipc->type == PARENT)
		sleep(5);
	else
		sem_lock(ipc->sem_id); // childs are waiting here
	sem_unlock(ipc->sem_id);
	sem_lock(ipc->sem_id);
	if (join_board(game))
	{
		ft_log(LOG_ERROR, "Failed to join board");
		return (1);
	}
	sem_unlock(ipc->sem_id);
	return (0);
}

static int	destroy_ipc(t_ipc *ipc)
{
	int	ret = 0;

	ft_log(LOG_DEBUG, "Deleting IPC\n");
	if (shm_destroy(ipc->shm_id))
		ret = 1;
	if (msg_queue_destroy(ipc->msg_id))
		ret = 1;
	if (sem_destroy(ipc->sem_id))
		ret = 1;
	return (ret);
}

int	close_ipc(t_ipc *ipc)
{
	int	nb_process = get_nb_process_attach(ipc->shm_id);
	if (nb_process == 2)
	{
		while ((nb_process = get_nb_process_attach(ipc->shm_id)) != 1)
			usleep(100);
	}
	shm_det(ipc->data);
	sem_lock(ipc->sem_id);
	sem_unlock(ipc->sem_id);
	if (nb_process == 1)
		if (destroy_ipc(ipc))
			return (1);
	return (0);
}