#include <player.h>
#include <string.h>

int	ipc_join_board(t_ipc *ipc, t_game *game)
{
	if (ipc->type == PARENT) {
		if (join_board(game))
			goto error;
		sem_unlock(ipc->sem_id);
		for (int cooldown = START_COOLDOWN; cooldown > 0; cooldown -= 1000000)
			usleep(1000000);
		sem_lock(ipc->sem_id);
		ipc->data->game_state ^= STARTED;
		sem_unlock(ipc->sem_id);
	} else {
		sem_lock(ipc->sem_id);
		if (join_board(game))
			goto error;
		while (!(ipc->data->game_state & STARTED)) {
			sem_unlock(ipc->sem_id);
			usleep(100000);
			sem_lock(ipc->sem_id);
		}
		sem_unlock(ipc->sem_id);
	}
	return (0);

	error:
	ERR("Failed to join board");
	sem_unlock(ipc->sem_id);
	return (1);
}

static int	destroy_ipc(t_ipc *ipc)
{
	DBG("Deleting IPC\n");
	return (sem_destroy(ipc->init_sem_id)
		|| shm_destroy(ipc->shm_id)
		|| msgq_destroy(ipc->msg_id)
		|| sem_destroy(ipc->sem_id));
}

uint32_t get_nb_visualizer_alive(t_ipc *ipc)
{
	uint32_t nb_msg = 0;
	while (check_msg(ipc->msg_id, NULL, 1, VISUALIZER_CHANNEL) == 1) {
		nb_msg++;
	}
	for (size_t i = 0; i < nb_msg; i++) {
		send_msg(ipc->msg_id, "*", 1, VISUALIZER_CHANNEL);
	}
	return (nb_msg);
}

int	close_ipc(t_ipc *ipc)
{
	sem_lock(ipc->sem_id);
	int	nb_process = get_attached_process_nb(ipc->shm_id);
	if (nb_process >= 2 && (uint32_t) nb_process == get_nb_visualizer_alive(ipc) + 1)
	{
		while ((nb_process = get_attached_process_nb(ipc->shm_id)) != 1)
		{
			sem_unlock(ipc->sem_id);
			usleep(100);
			sem_lock(ipc->sem_id);	
		}
	}
	DBG("Detaching from shared memory\n");
	shm_detach(ipc->data);
	DBG("Quiting with process = %d\n", nb_process);
	sem_unlock(ipc->sem_id);
	if (nb_process == 1)
		if (destroy_ipc(ipc))
			return (1);
	return (0);
}