#include <ipc.h>

uint32_t	get_shm_size(t_vec2 board_size)
{
	uint32_t	shmem_size = board_size.x * board_size.y * sizeof(uint32_t);
	return (PAGE_SIZE * ((shmem_size + PAGE_SIZE - 1) / PAGE_SIZE));
}

int	sem_lock(int sem_id)
{
	struct sembuf	ops;
	
	ops.sem_num = 0;
	ops.sem_op = -1;
	ops.sem_flg = 0;
	DBG("sem_lock\n");
	return (semop(sem_id, &ops, 1));
}

int	sem_lock_no_wait(int sem_id)
{
	struct sembuf	ops;
	
	ops.sem_num = 0;
	ops.sem_op = -1;
	ops.sem_flg = IPC_NOWAIT;
	return semop(sem_id, &ops, 1);
}

int	sem_unlock(int sem_id)
{
	struct sembuf	ops;
	
	ops.sem_num = 0;
	ops.sem_op = 1;
	ops.sem_flg = 0;
	DBG("sem_unlock");
	return (semop(sem_id, &ops, 1));
}

int	sem_destroy(int sem_id)
{
	if (semctl(sem_id, 0, IPC_RMID) == -1)
	{
		ERR("Can't destroy semaphore\n");
		return (1);
	}
	return (0);
}

int	get_nb_process_attach(int shm_id)
{
	struct shmid_ds	buf;
	if (shmctl(shm_id, IPC_STAT, &buf) == -1)
	{
		perror("shmctl");
		return (-1);
	}
	return (buf.shm_nattch);
}

int	shm_det(void *data)
{
	if (shmdt(data) == -1)
	{
		perror("semget");
		return (1);
	}
	return (0);
}

int	shm_destroy(int shm_id)
{
	if (shmctl(shm_id, IPC_RMID, NULL) == -1)
	{
		ERR("Can't destroy shared memory\n");
		return (1);
	}
	return (0);
}

int	msg_queue_destroy(int msg_id)
{
	if (msgctl(msg_id, IPC_RMID, NULL) == -1)
	{
		ERR("Can't destroy message queue\n");
		return (1);
	}
	return (0);
}