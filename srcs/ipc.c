#include "lem_ipc.h"

int	get_shm_id(key_t key, int flags)
{
	int		shm_id;
	
	shm_id = shmget(key, get_board_size_padded(), flags);
	if (shm_id == -1)
	{
		perror("shmget");
		return (-1);
	}
	return (shm_id);
}

int	*get_shm_data(int shm_id)
{
	int		*data;
	
	data = shmat(shm_id, NULL, 0);
	if (data == (void *)-1)
	{
		perror("shmat");
		shmctl(shm_id, IPC_RMID, NULL);
		return (NULL);
	}
	return (data);
}

int	init_semaphore(t_ipc *ipc)
{
	ipc->sem_id = semget(ipc->key, 1, IPC_CREAT | IPC_EXCL | 0644);
	if (ipc->sem_id == -1)
	{
		ipc->sem_id = semget(ipc->key, 1, 0644);
		if (ipc->sem_id == -1)
		{
			perror("semget");
			return (-1);
		}
		return (CHILD);
	}
	if (semctl(ipc->sem_id, 0, SETVAL, 0) == -1)
	{
		perror("semctl");
		return (-1);
	}
	return (PARENT);
}

int	init_msg_queue(key_t key)
{
	int		msg_id;
	
	msg_id = msgget(key, IPC_CREAT | 0644);
	if (msg_id == -1)
	{
		perror("msgget");
		return (-1);
	}
	return (msg_id);
}

int	init_ipc(t_ipc *ipc)
{
	char	*path = KEY_PATH;
	int		proj_id = 'R';
	
	ipc->key = ftok(path, proj_id);
	if (ipc->key == -1)
	{
		perror(path);
		return (1);
	}
	if ((ipc->type = init_semaphore(ipc)) == -1)
		return (1);
	if ((ipc->shm_id = get_shm_id(ipc->key, IPC_CREAT | 0644)) == -1)
		return (1);
	if (!(ipc->data = get_shm_data(ipc->shm_id)))
		return (1);
	if ((ipc->msg_id = init_msg_queue(ipc->key)) == -1)
		return (1);
	return (0);
}

int	ipc_join_board(t_ipc *ipc, t_game *game)
{
	if (ipc->type == PARENT)
		sleep(5);
	else
		sem_lock(ipc->sem_id);
	sem_unlock(ipc->sem_id);
	sem_lock(ipc->sem_id);
	if (join_board(game))
	{
		ft_printf_fd(2, "Failed to join board\n");
		return (1);
	}
	sem_unlock(ipc->sem_id);
	return (0);
}

void	wait_for_detach_display(t_ipc ipc)
{
	while (get_nb_process_attach(ipc.shm_id) == 2)
		usleep(100);
}

void	clear_ipc_data(t_ipc ipc)
{
	(void) ipc;
	ft_printf_fd(1, "Clearing IPC data\n");
}

int	is_display_running(t_ipc ipc)
{
	if (msgsnd(ipc.msg_id, &(t_msg){MSG_CHECK_DISPLAY, 0}, 0, 0) == -1)
	{
		perror("msgsnd");
		return (0);
	}
}

int	close_ipc(t_ipc ipc)
{
	int	nb_process = get_nb_process_attach(ipc.shm_id);
	if (nb_process == 2)
	{
		wait_for_detach_display(ipc);
		clear_ipc_data(ipc);
	}
	if (shmdt(ipc.data) == -1)
	{
		perror("semget");
		return (1);
	}
	return (0);
}