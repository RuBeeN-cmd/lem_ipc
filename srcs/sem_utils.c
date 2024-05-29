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