#include "lem_ipc.h"

int	ipc_join_board(t_ipc *ipc, t_game *game)
{
	if (ipc->type == PARENT)
		sleep(5);
	else
		sem_lock(ipc->sem_id); // childs are waiting here
	sem_unlock(ipc->sem_id);
	sem_lock(ipc->sem_id);
	if (join_board(game))
	{
		ft_log(LOG_ERROR, "Failed to join board");
		return (1);
	}
	sem_unlock(ipc->sem_id);
	return (0);
}

int	is_visualizer(t_ipc ipc)
{
	t_msg	msg;

	ft_bzero(&msg, sizeof(t_msg));
	msg.type = UINT32_MAX;
	char	text[] = "Hello, world !";
	ft_strlcpy(msg.text, text, sizeof(text));
	if (msgsnd(ipc.msg_id, &msg, sizeof(msg.text), 0) == -1)
	{
		perror("msgsnt");
		return (-1);
	}

	usleep(100000); // to give time to visualizer to catch it

	if (msgrcv(ipc.msg_id, &msg, sizeof(msg.text), UINT32_MAX, IPC_NOWAIT) == -1)
	{
		if (errno == ENOMSG)
		{
			return (1);
		}
		perror("msgrcv");
		return (-1);
	}
	return (0);
}

int	destroy_ipc(t_ipc ipc)
{
	int	ret = 0;

	ft_log(LOG_DEBUG, "Deleting IPC\n");
	if (shm_destroy(ipc.shm_id))
		ret = 1;
	if (msg_queue_destroy(ipc.msg_id))
		ret = 1;
	if (sem_destroy(ipc.sem_id))
		ret = 1;
	return (ret);
}

int	close_ipc(t_ipc ipc)
{
	int	nb_process = get_nb_process_attach(ipc.shm_id);
	if (nb_process == 2 && is_visualizer(ipc))
	{
		while ((nb_process = get_nb_process_attach(ipc.shm_id)) != 1)
			usleep(100);
	}
	shm_det(ipc.data);
	if (nb_process == 1)
		if (destroy_ipc(ipc))
			return (1);
	return (0);
}