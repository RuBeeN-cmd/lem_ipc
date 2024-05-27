#include "lem_ipc.h"

int	init_visualizer_semaphore(t_ipc *ipc)
{
	ipc->sem_id = semget(ipc->key, 1, 0);
	if (ipc->sem_id == -1)
		return (1);
	return (0);
}

int	init_visualizer_ipc(t_ipc *ipc)
{
	char	*path = "/Users/rubenrollin/Documents/Ruben/Projects/42/lem_ipc";
	int		proj_id = 'R';
	
	ipc->key = ftok(path, proj_id);
	if (ipc->key == -1)
	{
		perror(path);
		return (1);
	}
	if (init_visualizer_semaphore(ipc) || !(ipc->data = get_shm_data(ipc->key, 0)))
	{
		ft_printf_fd(2, "Nothing to visualize\n");
		return (1);
	}
	return (0);
}

int	init_visualizer(t_visualizer *visualizer)
{
	if (init_visualizer_ipc(&visualizer->ipc))
		return (1);
	init_board(visualizer->board, visualizer->ipc.data);
	visualizer->running = 1;
	visualizer->offset = init_vec2(WIN_WIDTH / 2, WIN_HEIGHT / 2);
	visualizer->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "Lem-Ipc", false);
	if (!visualizer->mlx)
		return (1);
	visualizer->scale = 1;
	visualizer->img = mlx_new_image(visualizer->mlx, visualizer->mlx->width, visualizer->mlx->height);
	if (!visualizer->img)
	{
		mlx_terminate(visualizer->mlx);
		return (1);
	}
	if (mlx_image_to_window(visualizer->mlx, visualizer->img, 0, 0) == -1)
	{
		mlx_terminate(visualizer->mlx);
		return (1);
	}
	mlx_loop_hook(visualizer->mlx, &render, visualizer);
	return (0);
}

uint32_t	get_team_color(int team)
{
	int		team_color = 0;
	switch (team)
	{
		case 1:
			team_color = 0xFFFF0000;
			break;
		case 2:
			team_color = 0xFF00FF00;
			break;
		case 3:
			team_color = 0xFF0000FF;
			break;
		default:
			team_color = 0xFFFFFFFF;
			break;
	}
	return (team_color);
}

void	draw_cell(t_visualizer *visualizer, int x, int y, int team)
{
	uint32_t	team_color = get_team_color(team);
	size_t		cell_size = visualizer->scale * CELL_SIZE;
	t_vec2		pos = init_vec2(visualizer->offset.x + (x - BOARD_WIDTH / 2.0) * cell_size, visualizer->offset.y + (y - BOARD_HEIGHT / 2.0) * cell_size);
	for (size_t i = 0; i < cell_size; i++)
	{
		for (size_t j = 0; j < cell_size; j++)
		{
			int color = team_color; 
			if (i == cell_size - 1 || j == cell_size - 1)
				color = 0xFF000000;
			if (pos.x >= 0 && pos.x < (int) visualizer->img->width && pos.y >= 0 && pos.y < (int) visualizer->img->height)
				((uint32_t *) visualizer->img->pixels)[pos.y * visualizer->img->width + pos.x] = color;
			pos.x++;
		}
		pos.x -= cell_size;
		pos.y++;
	}
}

void	draw_empty_board(t_visualizer *visualizer)
{
	for (size_t y = 0; y < BOARD_HEIGHT; y++)
		for (size_t x = 0; x < BOARD_WIDTH; x++)
			draw_cell(visualizer, x, y, 0);
}

int	draw_board(t_visualizer *visualizer)
{
	if (sem_lock_no_wait(visualizer->ipc.sem_id) != -1)
	{
		for (size_t y = 0; y < BOARD_HEIGHT; y++)
		{
			if (!visualizer->board[y])
			{
				sem_unlock(visualizer->ipc.sem_id);
				return (1);
			}
			for (size_t x = 0; x < BOARD_WIDTH; x++)
				draw_cell(visualizer, x, y, visualizer->board[y][x]);
		}
		sem_unlock(visualizer->ipc.sem_id);
	}
	else
		draw_empty_board(visualizer);
	return (0);
}

void	clear_window(mlx_image_t *img, int color)
{
	for (size_t i = 0; i < img->width * img->height; i++)
		((uint32_t *) img->pixels)[i] = color;
}

void	render(void *param)
{
	t_visualizer *visualizer = (t_visualizer *) param;
	handle_keys(visualizer);
	clear_window(visualizer->img, 0xFF000000);
	if (draw_board(visualizer))
		mlx_terminate(visualizer->mlx);
}

void	handle_keys(t_visualizer* visualizer)
{
	if (mlx_is_key_down(visualizer->mlx, MLX_KEY_W))
		visualizer->offset.y += MOVE_SPEED;
	if (mlx_is_key_down(visualizer->mlx, MLX_KEY_A))
		visualizer->offset.x += MOVE_SPEED;
	if (mlx_is_key_down(visualizer->mlx, MLX_KEY_S))
		visualizer->offset.y -= MOVE_SPEED;
	if (mlx_is_key_down(visualizer->mlx, MLX_KEY_D))
		visualizer->offset.x -= MOVE_SPEED;
	if (mlx_is_key_down(visualizer->mlx, MLX_KEY_UP))
	{
		visualizer->scale *= 1.01 * ZOOM_SPEED;
		if (visualizer->scale > 4)
			visualizer->scale = 4;
	}
	if (mlx_is_key_down(visualizer->mlx, MLX_KEY_DOWN))
	{
		visualizer->scale /= 1.01 * ZOOM_SPEED;
		if (visualizer->scale < 0.2)
			visualizer->scale = 0.2;
	}
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_visualizer *visualizer = (t_visualizer *) param;
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
	{
		if (visualizer->running)
		{
			visualizer->running = 0;
			sem_lock(visualizer->ipc.sem_id);
		}
		else
		{
			visualizer->running = 1;
			sem_unlock(visualizer->ipc.sem_id);
		}
	}
}