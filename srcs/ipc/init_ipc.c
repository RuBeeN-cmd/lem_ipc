#include <ipc.h>

static key_t	get_key(char *path, int proj_id)
{
	key_t	key;

	if ((key = ftok(path, proj_id)) == -1)
		ft_log(LOG_ERROR, "Can't create key from ftok\n");
	return (key);
}

static int	init_semaphore(t_ipc *ipc)
{
	ipc->sem_id = semget(ipc->key, 1, IPC_CREAT | IPC_EXCL | 0644);
	if (ipc->sem_id == -1)
	{
		ipc->sem_id = semget(ipc->key, 1, 0644);
		if (ipc->sem_id == -1)
		{
			ft_log(LOG_ERROR, "Can't get existing semaphore");
			return (-1);
		}
		return (CHILD);
	}
	return (PARENT);
}

static int	init_visualizer_semaphore(t_ipc *ipc)
{
	ipc->sem_id = semget(ipc->key, 1, 0);
	if (ipc->sem_id == -1)
		return (1);
	return (0);
}

static int	get_shm_id(key_t key, int flags, t_vec2 board_size)
{
	int		shm_id;
	
	shm_id = shmget(key, get_shm_size(board_size), flags);
	if (shm_id == -1)
	{
		ft_log(LOG_ERROR, "Can't access shared memory");
		return (-1);
	}
	return (shm_id);
}

static int	*get_shm_data(int shm_id)
{
	int		*data;
	
	data = shmat(shm_id, NULL, 0);
	if (data == (void *)-1)
	{
		ft_log(LOG_ERROR, "Can't attach shared memory");
		return (NULL);
	}
	return (data);
}

static int	init_msg_queue(key_t key)
{
	int		msg_id;
	
	msg_id = msgget(key, IPC_CREAT | 0644);
	if (msg_id == -1)
	{
		ft_log(LOG_ERROR, "Can't access message queue");
		return (-1);
	}
	return (msg_id);
}

static int	send_board_size(int msg_id, t_vec2 board_size)
{
	if (send_msg(msg_id, &board_size, sizeof(t_vec2), BOARD_SIZE_CHANNEL))
		return (1);
	return (0);
}

static t_vec2	get_board_size(int msg_id)
{
	t_vec2	board_size;
	int		ret;
	while ((ret = check_msg(msg_id, &board_size, sizeof(t_vec2), BOARD_SIZE_CHANNEL)) == 0) ;
	if (ret == -1)
		return (NULL_SIZE);
	if (send_board_size(msg_id, board_size))
		return (NULL_SIZE);
	return (board_size);
}

int	init_player_ipc(t_ipc *ipc, t_vec2 board_size)
{
	if ((ipc->key = get_key(KEY_PATH, 'R')) == -1)
		return (1);
	if ((ipc->type = init_semaphore(ipc)) == -1
		|| (ipc->msg_id = init_msg_queue(ipc->key)) == -1)
		return (1);
	if (ipc->type == PARENT && send_board_size(ipc->msg_id, board_size))
		return (1);
	else if (ipc->type == CHILD)
		board_size = get_board_size(ipc->msg_id);
	if (!vec2cmp(board_size, NULL_SIZE))
		return (1);
	if ((ipc->shm_id = get_shm_id(ipc->key, IPC_CREAT | 0644, board_size)) == -1
		|| !(ipc->data = get_shm_data(ipc->shm_id)))
		return (1);
	return (0);
}

int	init_visualizer_ipc(t_ipc *ipc, t_vec2 *board_size)
{
	if ((ipc->key = get_key(KEY_PATH, 'R')) == -1) {
		ft_log(LOG_ERROR, "Can't create key from ftok\n");
		return (1);
	}
	if (init_visualizer_semaphore(ipc)
		|| (ipc->msg_id = init_msg_queue(ipc->key)) == -1) {
		ft_log(LOG_ERROR, "Can't access message queue\n");
		return (1);
	}
	*board_size = get_board_size(ipc->msg_id);
	if (!vec2cmp(*board_size, NULL_SIZE)) {
		ft_log(LOG_ERROR, "Can't get board size\n");
		return (1);
	}
	if ((ipc->shm_id = get_shm_id(ipc->key, 0, *board_size)) == -1
		|| !(ipc->data = get_shm_data(ipc->shm_id))) {
		ft_log(LOG_ERROR, "Can't attach shared memory\n");
		return (1);
	}
	return (0);
}