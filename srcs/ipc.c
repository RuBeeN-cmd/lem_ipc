#include "lem_ipc.h"

void	sem_lock(int sem_id)
{
	struct sembuf	ops;
	
	ops.sem_num = 0;
	ops.sem_op = -1;
	ops.sem_flg = 0;
	if (semop(sem_id, &ops, 1) == -1)
	{
		perror("semop");
		exit(1);
	}
}

int	sem_lock_no_wait(int sem_id)
{
	struct sembuf	ops;
	
	ops.sem_num = 0;
	ops.sem_op = -1;
	ops.sem_flg = IPC_NOWAIT;
	return semop(sem_id, &ops, 1);
}

void	sem_unlock(int sem_id)
{
	struct sembuf	ops;
	
	ops.sem_num = 0;
	ops.sem_op = 1;
	ops.sem_flg = 0;
	if (semop(sem_id, &ops, 1) == -1)
	{
		perror("semop");
		exit(1);
	}
}

int	*get_shm_data(key_t key, int flags)
{
	int		shmid;
	int		*data;
	
	shmid = shmget(key, get_board_size_padded(), flags);
	if (shmid == -1)
	{
		perror("shmget");
		return (NULL);
	}
	data = shmat(shmid, NULL, 0);
	if (data == (void *)-1)
	{
		perror("shmat");
		shmctl(shmid, IPC_RMID, NULL);
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
	return (PARENT);
}

int	init_ipc(t_ipc *ipc)
{
	char	*path = "/Users/rubenrollin/Documents/Ruben/Projects/42/lem_ipc";
	int		proj_id = 'R';
	
	ipc->key = ftok(path, proj_id);
	if (ipc->key == -1)
	{
		perror(path);
		return (1);
	}
	if ((ipc->type = init_semaphore(ipc)) == -1)
		return (1);
	if (!(ipc->data = get_shm_data(ipc->key, IPC_CREAT | 0644)))
		return (1);
	return (0);
}

void	ipc_join_board(t_ipc *ipc, t_game *game)
{
	if (ipc->type == PARENT)
	{
		join_board(game);
		sleep(5);
		sem_unlock(ipc->sem_id);
	}
	else
	{
		sem_lock(ipc->sem_id);
		join_board(game);
		sem_unlock(ipc->sem_id);
	}
}