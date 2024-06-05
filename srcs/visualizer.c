#include "lem_ipc.h"

int	init_visualizer(t_visualizer *v)
{
	if (init_visualizer_ipc(&v->ipc))
		return (1);
	if (init_buffer(&v->buffer))
	{
		free_buffer(v->buffer, BOARD_HEIGHT);
		return (1);
	}
	v->running = 1;
	v->offset = init_vec2(WIN_WIDTH / 2, WIN_HEIGHT / 2);
	v->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "Lem-Ipc", false);
	if (!v->mlx)
		return (1);
	v->scale = 1;
	v->img = mlx_new_image(v->mlx, v->mlx->width, v->mlx->height);
	if (!v->img)
	{
		mlx_terminate(v->mlx);
		free_buffer(v->buffer, BOARD_HEIGHT);
		return (1);
	}
	if (mlx_image_to_window(v->mlx, v->img, 0, 0) == -1)
	{
		mlx_terminate(v->mlx);
		free_buffer(v->buffer, BOARD_HEIGHT);
		return (1);
	}
	mlx_key_hook(v->mlx, &key_hook, v);
	mlx_loop_hook(v->mlx, &visualizer_loop, v);
	return (0);
}

int	check_msg(t_ipc ipc)
{
	t_msg	msg;
	if (msgrcv(ipc.msg_id, &msg, sizeof(msg.text), UINT32_MAX, IPC_NOWAIT) == -1)
	{
		if (errno == ENOMSG)
		{
			// ft_printf_fd(1, C_RED"No Message\n"C_END);
			return (0);
		}
		perror("msgrcv");
		return (-1);
	}
	return (1);
}

void	visualizer_loop(void *param)
{
	t_visualizer *v = (t_visualizer *) param;
	handle_keys(v);
	if (sem_lock_no_wait(v->ipc.sem_id) != -1)
	{
		copy_buffer(v->buffer, v->ipc.data, BOARD_HEIGHT);
		sem_unlock(v->ipc.sem_id);
	}
	clear_window(v->img, 0xFF000000);
	draw_board(v);
	check_msg(v->ipc);
}

int	visualizer_workflow(void)
{
	t_visualizer	v;

	if (init_visualizer(&v))
		return (1);
	mlx_loop(v.mlx);
	mlx_terminate(v.mlx);
	free_buffer(v.buffer, BOARD_HEIGHT);
	return (0);
}

void	handle_keys(t_visualizer* v)
{
	if (mlx_is_key_down(v->mlx, MLX_KEY_W))
		v->offset.y += MOVE_SPEED;
	if (mlx_is_key_down(v->mlx, MLX_KEY_A))
		v->offset.x += MOVE_SPEED;
	if (mlx_is_key_down(v->mlx, MLX_KEY_S))
		v->offset.y -= MOVE_SPEED;
	if (mlx_is_key_down(v->mlx, MLX_KEY_D))
		v->offset.x -= MOVE_SPEED;
	if (mlx_is_key_down(v->mlx, MLX_KEY_UP))
	{
		v->scale *= 1.01 * ZOOM_SPEED;
		if (v->scale > 4)
			v->scale = 4;
	}
	if (mlx_is_key_down(v->mlx, MLX_KEY_DOWN))
	{
		v->scale /= 1.01 * ZOOM_SPEED;
		if (v->scale < 0.2)
			v->scale = 0.2;
	}
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_visualizer *v = (t_visualizer *) param;
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
	{
		if (v->running)
		{
			v->running = 0;
			sem_lock(v->ipc.sem_id);
		}
		else
		{
			v->running = 1;
			sem_unlock(v->ipc.sem_id);
		}
	}
}