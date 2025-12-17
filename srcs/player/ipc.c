#include <player.h>
#include <string.h>

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
		ERR("Failed to join board");
		sem_unlock(ipc->sem_id);
		return (1);
	}
	sem_unlock(ipc->sem_id);
	return (0);
}

static int	destroy_ipc(t_ipc *ipc)
{
	DBG("Deleting IPC\n");
	return (sem_destroy(ipc->init_sem_id)
		|| shm_destroy(ipc->shm_id)
		|| msg_queue_destroy(ipc->msg_id)
		|| sem_destroy(ipc->sem_id));
}

int is_visualizer(t_ipc *ipc)
{
	if (check_msg(ipc->msg_id, NULL, 1, VISUALIZER_CHANNEL) == 1)
	{
		send_msg(ipc->msg_id, "*", 1, VISUALIZER_CHANNEL);
		return (1);
	}
	return (0);
}

int	close_ipc(t_ipc *ipc)
{
	sem_lock(ipc->sem_id);
	int	nb_process = get_nb_process_attach(ipc->shm_id);
	DBG("Getting nb_process: %d\n", nb_process);
	if (nb_process == 2 && is_visualizer(ipc))
	{
		while ((nb_process = get_nb_process_attach(ipc->shm_id)) != 1)
		{
			DBG("Getting nb_process: %d\n", nb_process);
			sem_unlock(ipc->sem_id);
			usleep(100);
			sem_lock(ipc->sem_id);	
		}
	}
	DBG("Detaching from shared memory\n");
	shm_det(ipc->data);
	DBG("Quiting with process = %d\n", nb_process);
	sem_unlock(ipc->sem_id);
	if (nb_process == 1)
		if (destroy_ipc(ipc))
			return (1);
	return (0);
}