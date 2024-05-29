#include "lem_ipc.h"

int	*get_shm_data(key_t key, int flags, int sem_id, int wait)
{
	int		shmid;
	int		*data;
	
	shmid = shmget(key, get_board_size_padded(), flags);
	if (shmid == -1)
	{
		perror("shmget");
		return (NULL);
	}
	if (!wait)
		sem_lock(sem_id);
	data = shmat(shmid, NULL, 0);
	if (data == (void *)-1)
	{
		perror("shmat");
		shmctl(shmid, IPC_RMID, NULL);
		return (NULL);
	}
	if (wait)
		sleep(wait);
	sem_unlock(sem_id);
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
	if (!(ipc->data = get_shm_data(ipc->key, IPC_CREAT | 0644, ipc->sem_id, ipc->type == PARENT ? 5 : 0)))
		return (1);
	if ((ipc->msg_id = init_msg_queue(ipc->key)) == -1)
		return (1);
	return (0);
}

int	ipc_join_board(t_ipc *ipc, t_game *game)
{
	sem_lock(ipc->sem_id);
	if (join_board(game))
	{
		ft_printf_fd(2, "Failed to join board\n");
		return (1);
	}
	sem_unlock(ipc->sem_id);
	return (0);
}

/*
P	: Lock1 - JoinBoard - Unlock1 - Lock2 - Sleep 5 ------------------------------------------- Unlock2 -----------------------------------
C1	: ----------------------------- Lock1 - JoinBoard - Unlock1 ----------------------------------------- Lock2 - Unlock2 -----------------
C2	: ----------------------------------------------------------- Lock1 - JoinBoard - Unlock1 ----------------------------- Lock2 - Unlock2



P	: JoinBoard - Lock - Sleep 5 - Unlock -------------------------------------
C1	: JoinBoard --------------------------- Lock - Unlock ---------------------
C2	: JoinBoard ------------------------------------------- Lock - Unlock -----



P	: Lock - JoinBoard - Unlock - Sleep 5 ------------------------------------------------------------------------------- Lock - Unlock
C1	: --------------------------- Lock - JoinBoard - Unlock - Lock - Unlock -----------------------------------------------------------
C2	: ----------------------------------------------------------------------- Lock - JoinBoard - Unlock - Lock - Unlock ---------------



P	: Lock - JoinBoard - Sleep 5 - Unlock -------------------------------------------------------
C1	: ------------------------------------- Lock - JoinBoard - Unlock ---------------------------
C2	: ----------------------------------------------------------------- Lock - JoinBoard - Unlock




P	: Lock - AttachMem - Sleep 5 ------------------------------------------------------- Unlock ----------------------------------------- Lock - JoinBoard - Unlock -------------------------------------------------------
C1	: ---------------------------- Lock -------------------------------------------------------- AttachMem - Unlock ------------------------------------------------- Lock - JoinBoard - Unlock ---------------------------
C2	: ---------------------------- Lock ---------------------------------------------------------------------------- AttachMem - Unlock --------------------------------------------------------- Lock - JoinBoard - Unlock
  


*/