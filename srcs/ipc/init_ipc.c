#include <ipc.h>

static key_t	get_key(char *path, int proj_id)
{
	key_t	key;

	if ((key = ftok(path, proj_id)) == -1)
		ERR("Can't create key from ftok\n");
	return (key);
}

static int	init_semaphore(key_t key, int flags)
{
	return (semget(key, 1, flags));
}

static int	get_shm_id(key_t key, int flags, uint32_t shm_size)
{
	return (shmget(key, shm_size, flags));
}
	
static void	*get_shm_data(int shm_id)
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
	DBG("Sending board size: (%d, %d) in msg_q %d\n", board_size.x, board_size.y, msg_id);
	if (send_msg(msg_id, &board_size, sizeof(t_vec2), BOARD_SIZE_CHANNEL))
		return (1);
	return (0);
}

static t_vec2	get_board_size(int msg_id)
{
	t_vec2	board_size;
	DBG("Getting board size from msg_q %d\n", msg_id);
	if (check_msg(msg_id, &board_size, sizeof(t_vec2), BOARD_SIZE_CHANNEL) != 1)
		return (NULL_SIZE);
	return (board_size);
}

static int	init_parent(t_ipc *ipc, t_vec2 board_size)
{
	if ((ipc->sem_id = init_semaphore(ipc->key, IPC_CREAT | IPC_EXCL | 0644)) == -1) {
		ERR("Can't create semaphore.\n");
		goto error;
	}
	if ((ipc->msg_id = init_msg_queue(ipc->key, IPC_CREAT | IPC_EXCL | 0644)) == -1) {
		ERR("Can't create message queue.\n");
		goto destroy_sem;
	}
	if (send_board_size(ipc->msg_id, board_size))
	{
		ERR("Failed to send board size.\n");
		goto destroy_msg_q;
	}
	if ((ipc->shm_id = get_shm_id(ipc->key, IPC_CREAT | IPC_EXCL | 0644, get_shm_size(board_size))) == -1) {
		ERR("Can't create shared memory.\n");
		goto destroy_shm;
	}
	if (!(ipc->data = (t_shm_hdr *) get_shm_data(ipc->shm_id))) {
		ERR("Can't attach shared memory.\n");
		goto destroy_shm;
	}
	DBG("Shared Memory address: %p\n", ipc->data);
	return (0);

	destroy_shm: shm_destroy(ipc->shm_id);
	destroy_msg_q: msg_queue_destroy(ipc->msg_id);
	destroy_sem: sem_destroy(ipc->sem_id);
	error: return (1);
}

static void clean_msg_queue(t_ipc *ipc)
{
	int is_message = 1;
	while (is_message)
		if (check_msg(ipc->msg_id, NULL, 1, 0) == 0)
			is_message = 0;
}


static int	init_child(t_ipc *ipc, t_vec2 *board_size)
{
	if ((ipc->sem_id = init_semaphore(ipc->key, 0644)) == -1) {
		ERR("Can't get semaphore.\n");
		return (1);
	}
	if ((ipc->msg_id = init_msg_queue(ipc->key, 0644)) == -1) {
		ERR("Can't get message queue.\n");
		return (1);
	}
	*board_size = get_board_size(ipc->msg_id);
	if (!vec2cmp(*board_size, NULL_SIZE))
	{
		ERR("Failed to fetch board size.\n");
		return (1);
	}
	DBG("Board size received: (%d, %d)\n", board_size->x, board_size->y);
	if (send_board_size(ipc->msg_id, *board_size)) {
		WARN("Failed to send board size, cleaning message queue\n");
		uint32_t msg_q_size = message_queue_size_get(ipc->msg_id);
		DBG("Message queue size before : %u\n", msg_q_size);
		clean_msg_queue(ipc);
		msg_q_size = message_queue_size_get(ipc->msg_id);
		DBG("Message queue size after: %u\n", msg_q_size);
		send_board_size(ipc->msg_id, *board_size);
	}
	if ((ipc->shm_id = get_shm_id(ipc->key, 0, get_shm_size(*board_size))) == -1) {
		ERR("Can't get shared memory.\n");
		return (1);
	}
	if (!(ipc->data = get_shm_data(ipc->shm_id))) {
		ERR("Can't attach shared memory.\n");
		return (1);
	}
	DBG("Shared Memory address: %p\n", ipc->data);
	return (0);
}

static int init_keys(t_ipc *ipc)
{
	if ((ipc->init_key = get_key(KEY_PATH, INIT_PROJ_ID)) == -1) {
		ERR("Can't create initialization key from ftok.\n");
		return (1);
	}
	if ((ipc->key = get_key(KEY_PATH, SHM_PROJ_ID)) == -1) {
		ERR("Can't create key from ftok.\n");
		return (1);
	}
	return (0);
}

static int	init_initialization(t_ipc *ipc, int is_visualizer)
{
	if (is_visualizer || (ipc->init_sem_id = init_semaphore(ipc->init_key, IPC_CREAT | IPC_EXCL | 0644)) == -1) {
		if ((ipc->init_sem_id = init_semaphore(ipc->init_key, 0644)) == -1)
			return (1);
		ipc->type = CHILD;
		return (0);
	}
	ipc->type = PARENT;
	return (0);
}

int	init_player_ipc(t_ipc *ipc, t_vec2 *board_size)
{
	if (init_keys(ipc))
		return (1);
	if (init_initialization(ipc, 0)) {
		ERR("Failed to access initialization semaphore.\n");
		return (1);
	}
	if (ipc->type == PARENT) {
		if (init_parent(ipc, *board_size))
			goto error;
	} else if (ipc->type == CHILD) {
		sem_lock(ipc->init_sem_id);
		if (init_child(ipc, board_size))
			goto error;
	}
	
	DBG("SHM_DATA_SIZE: %d\n", ((int) SHM_DATA_SIZE(*board_size)));
	DBG("SHM_BOARD_OFFSET: %d\n", SHM_BOARD_OFFSET);
	sem_unlock(ipc->init_sem_id);
	return (0);

	error: sem_unlock(ipc->init_sem_id);
	return (1);
}

int	init_visualizer_ipc(t_ipc *ipc, t_vec2 *board_size)
{
	if (init_keys(ipc))
		return (1);
	if (init_initialization(ipc, 1)) {
		ERR("No players fighting.\n");
		return (1);
	}
	sem_lock(ipc->init_sem_id);
	if (init_child(ipc, board_size))
		goto error;
	sem_unlock(ipc->init_sem_id);
	return (0);

	error: sem_unlock(ipc->init_sem_id);
	return (1);
}
