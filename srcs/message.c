#include "lem_ipc.h"

int	send_pause_msg(int msg_id)
{
	t_msg	msg = {
		PAUSE_CHANNEL,
		"*"
	};
	if (msgsnd(msg_id, &msg, sizeof(msg), 0) == -1)
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
    t_msg	msg;

    msg.type = VISUALIZER_TARGET_CHANNEL;
    
    ft_memcpy((msg.text), &target, 8);

    ft_printf_fd(1, "Msg: %p\n", &msg);
    ft_printf_fd(1, "Msg.type: %p\n", &(msg.type));
    ft_printf_fd(1, "Text: %p\n", msg.text);

    for (size_t i = 0; i < 8; i++)
    {
        ft_printf_fd(1, "char[%u] = %d\n | %p\n", i, msg.text[i], msg.text + i);
    }



    // msg.v.x = target.x;
    // msg.v.y = target.y;

	if (msgsnd(msg_id, &msg, 16, 0) == -1)
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
		{
            ft_printf_fd(1, "No msg\n");
            return (0);
		}
		ft_printf_fd(1, "Msgrcv Error\n");
		perror("msgrcv");
		return (-1);
	}
	ft_printf_fd(1, "New msg: %d, %d\n", msg.v.x, msg.v.y);
    target->x = msg.v.x;
    target->y = msg.v.y;
	return (1);
}