#include <ipc.h>

uint32_t	get_shm_size(t_vec2 board_size)
{
	return (PAGE_SIZE * ((SHM_DATA_SIZE(board_size) + PAGE_SIZE - 1) / PAGE_SIZE));
}

int	shm_detach(void *data)
{
	errno = 0;
	if (shmdt(data) == -1) {
		ERR("Failed to detach from shared memory: %s\n", strerror(errno));
		perror("semget");
		return (1);
	}
	return (0);
}

int	shm_destroy(int shm_id)
{
	errno = 0;
	if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
		ERR("Can't destroy shared memory: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

int	get_attached_process_nb(int shm_id)
{
	struct shmid_ds	buf = {};

	errno = 0;
	if (shmctl(shm_id, IPC_STAT, &buf) == -1) {
		ERR("Failed to retrieve attached process number: %s\n", strerror(errno));
		return (-1);
	}
	return (buf.shm_nattch);
}