#include <ipc.h>

static key_t	get_key(char *path, int proj_id)
{
	key_t	key;

	if ((key = ftok(path, proj_id)) == -1)
		ft_log(LOG_ERROR, "Can't create key from ftok\n");
	return (key);
}

static int	init_semaphore(key_t key, int flags)
{
	return (semget(key, 1, flags));
}

static int	get_shm_id(key_t key, int flags, t_vec2 board_size)
{
	return (shmget(key, get_shm_size(board_size), flags));
}

static int	*get_shm_data(int shm_id)
{
	int		*data;
	
	data = shmat(shm_id, NULL, 0);
	if (data == (void *)-1)
		return (NULL);
	return (data);
}

static int	init_msg_queue(key_t key, int flags)
{
	return (msgget(key, flags));
}

static int	send_board_size(int msg_id, t_vec2 board_size)
{
	ft_log(LOG_DEBUG, "Sending board size: (%d, %d) in msg_q %d", board_size.x, board_size.y, msg_id);
	if (send_msg(msg_id, &board_size, sizeof(t_vec2), BOARD_SIZE_CHANNEL))
		return (1);
	return (0);
}

static t_vec2	get_board_size(int msg_id)
{
	t_vec2	board_size;
	ft_log(LOG_DEBUG, "Getting board size from msg_q %d", msg_id);
	if (check_msg(msg_id, &board_size, sizeof(t_vec2), BOARD_SIZE_CHANNEL) != 1)
		return (NULL_SIZE);
	return (board_size);
}

static int	init_parent(t_ipc *ipc, t_vec2 board_size)
{
	if ((ipc->sem_id = init_semaphore(ipc->key, IPC_CREAT | IPC_EXCL | 0644)) == -1) {
		ft_log(LOG_ERROR, "Can't create semaphore.");
		goto error;
	}
	if ((ipc->msg_id = init_msg_queue(ipc->key, IPC_CREAT | IPC_EXCL | 0644)) == -1) {
		ft_log(LOG_ERROR, "Can't create message queue.");
		goto destroy_sem;
	}
	ft_log(LOG_DEBUG, "Message queue created: %d", ipc->msg_id);
	if (send_board_size(ipc->msg_id, board_size))
	{
		ft_log(LOG_ERROR, "Failed to send board size.");
		goto destroy_msg_q;
	}
	if ((ipc->shm_id = get_shm_id(ipc->key, IPC_CREAT | IPC_EXCL | 0644, board_size)) == -1) {
		ft_log(LOG_ERROR, "Can't create shared memory.");
		goto destroy_shm;
	}
	if (!(ipc->data = get_shm_data(ipc->shm_id))) {
		ft_log(LOG_ERROR, "Can't attach shared memory.");
		goto destroy_shm;
	}
	return (0);

	destroy_shm: shm_destroy(ipc->shm_id);
	destroy_msg_q: msg_queue_destroy(ipc->msg_id);
	destroy_sem: sem_destroy(ipc->sem_id);
	error: return (1);
}

static int	init_child(t_ipc *ipc, t_vec2 *board_size)
{
	if ((ipc->sem_id = init_semaphore(ipc->key, 0644)) == -1) {
		ft_log(LOG_ERROR, "Can't get semaphore.");
		return (1);
	}
	if ((ipc->msg_id = init_msg_queue(ipc->key, 0644)) == -1) {
		ft_log(LOG_ERROR, "Can't get message queue.");
		return (1);
	}
	*board_size = get_board_size(ipc->msg_id);
	if (!vec2cmp(*board_size, NULL_SIZE))
	{
		ft_log(LOG_ERROR, "Failed to fetch board size.");
		return (1);
	}
	ft_log(LOG_DEBUG, "Board size received: (%d, %d)", board_size->x, board_size->y);
	if (send_board_size(ipc->msg_id, *board_size))
	{
		ft_log(LOG_ERROR, "Failed to send board size.");
		return (1);
	}
	if ((ipc->shm_id = get_shm_id(ipc->key, 0, *board_size)) == -1) {
		ft_log(LOG_ERROR, "Can't get shared memory.");
		return (1);
	}
	if (!(ipc->data = get_shm_data(ipc->shm_id))) {
		ft_log(LOG_ERROR, "Can't attach shared memory.");
		return (1);
	}
	return (0);
}

static int	init_initialization(t_ipc *ipc)
{
	if ((ipc->init_key = get_key(KEY_PATH, INIT_PROJ_ID)) == -1) {
		ft_log(LOG_ERROR, "Can't create initialization key from ftok.");
		return (1);
	}
	if ((ipc->key = get_key(KEY_PATH, SHM_PROJ_ID)) == -1) {
		ft_log(LOG_ERROR, "Can't create key from ftok.");
		sem_unlock(ipc->init_sem_id);
		return (1);
	}
	if (init_semaphore(ipc->init_key, IPC_CREAT | IPC_EXCL | 0644) == -1) {
		if (init_semaphore(ipc->init_key, 0644) == -1) {
			ft_log(LOG_ERROR, "Can't access initialization semaphore.");
			return (1);
		}
		ipc->type = CHILD;
		return (0);
	}
	ipc->type = PARENT;
	return (0);
}

int	init_player_ipc(t_ipc *ipc, t_vec2 *board_size)
{
	if (init_initialization(ipc))
		return (1);
	if (ipc->type == PARENT) {
		if (init_parent(ipc, *board_size))
			goto error;
	} else if (ipc->type == CHILD) {
		sem_lock(ipc->init_sem_id);
		if (init_child(ipc, board_size))
			goto error;
	}
	sem_unlock(ipc->init_sem_id);
	return (0);

	error: sem_unlock(ipc->init_sem_id);
	return (1);
}

int	init_visualizer_ipc(t_ipc *ipc, t_vec2 *board_size)
{
	if (init_initialization(ipc))
		return (1);
	sem_lock(ipc->init_sem_id);
	if (init_child(ipc, board_size))
		goto error;
	sem_unlock(ipc->init_sem_id);
	return (0);

	error: sem_unlock(ipc->init_sem_id);
	return (1);
}
