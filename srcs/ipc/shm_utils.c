#include <ipc.h>

void	resume_game(int sem_id, t_game_state *game_state)
{
	sem_lock(sem_id);
	*game_state = RUNNING;
	sem_unlock(sem_id);
}

void	pause_game(int sem_id, t_game_state *game_state)
{
	sem_lock(sem_id);
	*game_state = PAUSED;
	sem_unlock(sem_id);
}

t_game_state	get_game_state(int sem_id, t_game_state *game_state)
{
	t_game_state state;
	sem_lock(sem_id);
	state = *game_state;
	sem_unlock(sem_id);
	return (state);
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