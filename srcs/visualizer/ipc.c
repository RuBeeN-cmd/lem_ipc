#include <visualizer/visualizer.h>

void	try_sync_shm(t_visualizer *v)
{
	if (sem_lock_no_wait(v->ipc.sem_id) != -1)
	{
		copy_buffer(v->buffer, (uint32_t *) (v->ipc.data + SHM_BOARD_OFFSET), v->board_size);
		v->game_state = v->ipc.data->game_state;
		sem_unlock(v->ipc.sem_id);
	}
}