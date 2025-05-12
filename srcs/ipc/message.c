#include <ipc.h>

int	send_pause_msg(int msg_id)
{
	t_msg	msg = {
		PAUSE_CHANNEL,
		"*"
	};
	if (msgsnd(msg_id, &msg, 1, 0) == -1)
	{
		perror("msgsnt");
		return (1);
	}
	return (0);
}

int	check_pause_msg(int msg_id)
{
	t_msg	msg;

	errno = 0;
	if (msgrcv(msg_id, &msg, sizeof(msg), PAUSE_CHANNEL, IPC_NOWAIT) == -1)
	{
		if (errno == ENOMSG)
		{
			return (0);
		}
		ft_printf_fd(1, "Msgrcv Error\n");
		perror("msgrcv");
		return (-1);
	}
	return (1);
}

int send_visualizer_target_msg(int msg_id, t_vec2 target)
{
	t_vec2_msg	msg = {
		VISUALIZER_TARGET_CHANNEL,
		target
	};
	if (msgsnd(msg_id, &msg, sizeof(msg.v), 0) == -1)
	{
		perror("msgsnt");
		return (1);
	}
	ft_printf_fd(1, "Visualizer target msg send\n");
	return (0);
}

int	check_visualizer_target_msg(int msg_id, t_vec2 *target)
{
	t_vec2_msg	msg;

	errno = 0;
	if (msgrcv(msg_id, &msg, sizeof(msg), VISUALIZER_TARGET_CHANNEL, IPC_NOWAIT) == -1)
	{
		if (errno == ENOMSG)
			return (0);
		perror("msgrcv");
		return (-1);
	}
	target->x = msg.v.x;
	target->y = msg.v.y;
	return (1);
}

int send_target_infos_msg(int msg_id, t_supervised_infos target_infos)
{
	t_supervised_infos_msg	msg = {
		TARGET_INFOS_CHANNEL,
		target_infos
	};
	if (msgsnd(msg_id, &msg, sizeof(msg.target_infos), 0) == -1)
	{
		perror("msgsnt");
		return (1);
	}
	return (0);
}

int	check_target_infos_msg(int msg_id, t_supervised_infos *target_infos)
{
	t_supervised_infos_msg	msg;

	errno = 0;
	if (msgrcv(msg_id, &msg, sizeof(msg), TARGET_INFOS_CHANNEL, IPC_NOWAIT) == -1)
	{
		if (errno == ENOMSG)
			return (0);
		perror("msgrcv");
		return (-1);
	}
	target_infos->pos = msg.target_infos.pos;
	target_infos->team = msg.target_infos.team;
	target_infos->is_alive = msg.target_infos.is_alive;
	return (1);
}