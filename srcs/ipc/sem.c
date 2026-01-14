#include <ipc.h>

int	sem_lock(int sem_id)
{
	struct sembuf	ops;
	
	ops.sem_num = 0;
	ops.sem_op = -1;
	ops.sem_flg = 0;
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