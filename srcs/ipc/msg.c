#include <ipc.h>

t_new_target_msg	new_target_msg(t_vec2 target, t_new_target_msg_type type)
{
	return ((t_new_target_msg) {
		.type = type,
		.target = target
	});
}

void	*new_msg(void *data, uint32_t data_size, uint32_t channel)
{
	errno = 0;
	void	*msg = malloc(data_size + sizeof(uint64_t));
	if (msg == NULL) {
		ERR("Memory allocation failed: %s\n", strerror(errno));
		return (NULL);
	}
	*(uint64_t *)msg = channel;
	ft_memcpy(msg + sizeof(uint64_t), data, data_size);
	return (msg);
}

int	check_msg(int msg_id, void *data, uint32_t data_size, uint32_t channel)
{
	errno = 0;
	void	*msg = malloc(data_size + sizeof(uint64_t));
	if (msg == NULL) {
		ERR("Memory allocation failed: %s\n", strerror(errno));
		return (-1);
	}

	errno = 0;
	if (msgrcv(msg_id, msg, data_size, channel, IPC_NOWAIT) == -1) {
		if (errno == ENOMSG) {
			free(msg);
			return (0);
		}
		ERR("Failed to receive message: %s\n", strerror(errno));
		free(msg);
		return (-1);
	}
	if (data)
		ft_memcpy(data, msg + sizeof(uint64_t), data_size);
	free(msg);
	return (1);
}

int send_msg(int msg_id, void *data, uint32_t data_size, uint32_t channel)
{
	void *msg = new_msg(data, data_size, channel);
	if (!msg)
		return (-1);

	errno = 0;
	if (msgsnd(msg_id, msg, data_size, IPC_NOWAIT) == -1) {
		ERR("Failed to send message: %s\n", strerror(errno));
		free(msg);
		return (-1);
	}
	free(msg);
	return (0);
}

void clean_msg_queue(t_ipc *ipc)
{
	while (check_msg(ipc->msg_id, NULL, 1, 0) == 1) ;
}

uint32_t get_msgq_size(int msgid)
{
	struct msqid_ds buf = {};

	errno = 0;
	if (msgctl(msgid, IPC_STAT, &buf) == -1) {
		ERR("Failed to retrieve message queue size: %s\n", strerror(errno));
		return (0);
	}
	return (buf.__msg_cbytes);
}

int	msgq_destroy(int msg_id)
{
	errno = 0;
	if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
		ERR("Failed to destroy message queue: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}