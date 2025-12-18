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
		free(msg_ptr);
		perror("msgsnt");
		return (1);
	}
	free(msg_ptr);
	return (0);
}