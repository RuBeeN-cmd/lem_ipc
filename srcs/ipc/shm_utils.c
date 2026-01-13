#include <ipc.h>

void	resume_game(t_game_state *game_state)
{
	*game_state = RUNNING;
}

void	pause_game(t_game_state *game_state)
{
	*game_state = PAUSED;
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