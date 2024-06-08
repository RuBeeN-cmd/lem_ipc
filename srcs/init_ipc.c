#include "lem_ipc.h"

key_t	get_key(char *path, int proj_id)
{
	key_t	key;

	if ((key = ftok(path, proj_id)) == -1)
		ft_log(LOG_ERROR, "Can't create key from ftok\n");
	return (key);
}

int	init_semaphore(t_ipc *ipc)
{
	ipc->sem_id = semget(ipc->key, 1, IPC_CREAT | IPC_EXCL | 0644);
	if (ipc->sem_id == -1)
	{
		ipc->sem_id = semget(ipc->key, 1, 0644);
		if (ipc->sem_id == -1)
		{
			ft_log(LOG_ERROR, "Can't get existing semaphore");
			return (-1);
		}
		return (CHILD);
	}
	return (PARENT);
}

int	get_shm_id(key_t key, int flags)
{
	int		shm_id;
	
	shm_id = shmget(key, get_board_size_padded(), flags);
	if (shm_id == -1)
	{
		ft_log(LOG_ERROR, "Can't access shared memory");
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
		ft_log(LOG_ERROR, "Can't attach shared memory");
		return (NULL);
	}
	return (data);
}

int	init_msg_queue(key_t key)
{
	int		msg_id;
	
	msg_id = msgget(key, IPC_CREAT | 0644);
	if (msg_id == -1)
	{
		ft_log(LOG_ERROR, "Can't access message queue");
		return (-1);
	}
	return (msg_id);
}

int	init_ipc(t_ipc *ipc)
{
	if ((ipc->key = get_key(KEY_PATH, 'R')) == -1)
		return (1);
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
