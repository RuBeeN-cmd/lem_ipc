#include <ipc.h>

t_new_target_msg	new_msg(t_vec2 target, t_new_target_msg_type type)
{
	return ((t_new_target_msg) {
		.type = type,
		.target = target
	});
}

int	check_msg(int msg_id, void *data, uint32_t data_size, uint32_t channel)
{
	void	*msg_ptr = malloc(data_size + sizeof(uint64_t));
	if (msg_ptr == NULL)
	{
		perror("malloc");
		return (-1);
	}
	errno = 0;
	if (msgrcv(msg_id, msg_ptr, data_size, channel, IPC_NOWAIT) == -1)
	{
		free(msg_ptr);
		if (errno == ENOMSG)
			return (0);
		perror("msgrcv");
		return (-1);
	}
	if (data)
		ft_memcpy(data, msg_ptr + sizeof(uint64_t), data_size);
	free(msg_ptr);
	return (1);
}

int send_msg(int msg_id, void *data, uint32_t data_size, uint32_t channel)
{
	void	*msg_ptr = malloc(data_size + sizeof(uint64_t));
	if (msg_ptr == NULL)
	{
		perror("malloc");
		return (-1);
	}
	*(uint64_t *)msg_ptr = channel;
	ft_memcpy(msg_ptr + sizeof(uint64_t), data, data_size);
	if (msgsnd(msg_id, msg_ptr, data_size, IPC_NOWAIT) == -1)
	{
		int err = errno;
		perror("msgsnt");
		free(msg_ptr);
		if (err != EAGAIN) {
			return (-1);
		}
		uint32_t msg_q_size = message_queue_size_get(msg_id);
		DBG("Message queue size : %u\n", msg_q_size);
		return (1);
	}
	free(msg_ptr);
	return (0);
}

uint32_t message_queue_size_get(int msgid)
{
	struct msqid_ds buf = {};

	errno = 0;
	if (msgctl(msgid, IPC_STAT, &buf) == -1) {
		// syscall_perror("msgctl");
		return (0);
	}
	return (buf.__msg_cbytes);
}