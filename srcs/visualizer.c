#include "lem_ipc.h"

int init_sdl(t_visualizer *v, char title[], uint32_t width, uint32_t height)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		perror("SDL");
		ft_log(LOG_ERROR, "Failed to init SDL !\n");
		return (1);
	}
	ft_log(LOG_DEBUG, "SDL successfully initialized !\n");
	v->window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
	if (!v->window)
	{
		ft_log(LOG_ERROR, "Failed to init window !\n");
		SDL_Quit();
		return (1);
	}
	ft_log(LOG_DEBUG, "Window successfully initialized !\n");
	v->renderer = SDL_CreateRenderer(v->window, NULL);
	if (!v->renderer)
	{
		ft_log(LOG_ERROR, "Failed to init renderer !\n");
		SDL_DestroyWindow(v->window);
		SDL_Quit();
		return (1);
	}
	ft_log(LOG_DEBUG, "Renderer successfully initialized !\n");
	return (0);
}

int	init_visualizer(t_visualizer *v, char title[], uint32_t width, uint32_t height)
{
	if (init_visualizer_ipc(&v->ipc))
		return (1);
	if (init_buffer(&v->buffer))
		return (1);
	v->running = 1;
	v->cell_size = (height - INITIAL_PADDING) / BOARD_HEIGHT;
	if (v->cell_size * BOARD_WIDTH > (width - INITIAL_PADDING))
		v->cell_size = (width - INITIAL_PADDING) / BOARD_WIDTH;
	v->offset = (t_fvec2) {0, 0};
	if (init_sdl(v, title, width, height))
	{
		free_buffer(v->buffer, BOARD_HEIGHT);
		return (1);
	}
	return (0);
}

int	hand_check_msg(t_ipc ipc)
{
	t_msg	msg;
	if (msgrcv(ipc.msg_id, &msg, sizeof(msg.text), UINT32_MAX, IPC_NOWAIT) == -1)
	{
		if (errno == ENOMSG)
		{
			ft_printf_fd(1, C_RED"No Message\n"C_END);
			return (0);
		}
		perror("msgrcv");
		return (-1);
	}
	ft_printf_fd(1, C_GRN"Message received\n"C_END);
	shm_det(ipc.data);
	return (1);
}

void	destroy_visualizer(t_visualizer v)
{
	SDL_DestroyRenderer(v.renderer);
	SDL_DestroyWindow(v.window);
	SDL_Quit();
	free_buffer(v.buffer, BOARD_HEIGHT);
	exit(0);
}

int	is_game_ended(uint32_t **board)
{
	uint32_t found_id = 0;
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (board[y][x] != EMPTY_CELL) {
				if (found_id == 0)
					found_id = board[y][x];
				else if (board[y][x] != found_id)
					return (1);
			}
		}
	}
	return (found_id == 0);
}

int	is_on_board(t_vec2 pos)
{
	return (pos.x >= 0 && pos.x < BOARD_WIDTH
		&& pos.y >= 0 && pos.y < BOARD_HEIGHT);
}

t_vec2	screen_pos_to_board_pos(t_visualizer *v, t_vec2 screen_pos)
{
	t_vec2 renderer_size;
	SDL_GetCurrentRenderOutputSize(v->renderer, &(renderer_size.x), &(renderer_size.y));

	t_vec2	v1 = sub_vec2(screen_pos, scalar_div_vec2(renderer_size, 2));
	t_fvec2	v2 = scalar_div_fvec2(vec2_to_fvec2(v1), v->cell_size);
	v2 = add_fvec2(v2, v->offset);
	v2 = add_fvec2(v2, scalar_div_fvec2((t_fvec2) {BOARD_WIDTH, BOARD_HEIGHT}, 2));
	v1 = fvec2_to_vec2(v2);
	if (is_on_board(v1))
		return (v1);
	return (NULL_POS);
}

int	handle_events(t_visualizer* v)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
			return (1);
		else if (e.type == SDL_EVENT_KEY_DOWN) {
			if (e.key.key == SDLK_ESCAPE)
				return (1);
			else if (e.key.key == SDLK_UP)
				v->cell_size *= 1.1;
			else if (e.key.key == SDLK_DOWN) {
				v->cell_size /= 1.1;
				if (v->cell_size < 4)
					v->cell_size = 4;
			}
			else if (e.key.key == SDLK_W)
				v->offset.y -= MOVE_SPEED / (float) v->cell_size;
			else if (e.key.key == SDLK_A)
				v->offset.x -= MOVE_SPEED / (float) v->cell_size;
			else if (e.key.key == SDLK_S)
				v->offset.y += MOVE_SPEED / (float) v->cell_size;
			else if (e.key.key == SDLK_D)
				v->offset.x += MOVE_SPEED / (float) v->cell_size;
			else if (e.key.key == SDLK_SPACE) {
				if (v->running) {
					v->running = 0;
					sem_lock(v->ipc.sem_id);
					ft_log(LOG_DEBUG, "Paused\n");
					send_pause_msg(v->ipc);
					sem_unlock(v->ipc.sem_id);
				} else {
					v->running = 1;
					sem_lock(v->ipc.sem_id);
					ft_log(LOG_DEBUG, "Unpaused\n");
					check_pause_msg(v->ipc);
					sem_unlock(v->ipc.sem_id);
				}
			}
		} else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				t_vec2	target = screen_pos_to_board_pos(v, (t_vec2) {e.button.x, e.button.y});
				if (vec2cmp(target, NULL_POS))
				{
					ft_printf_fd(1, "Clicked on : (%d, %d)\n", target.x, target.y);		
				}
			}
		}
	}
	return (0);
}

int	visualizer_routine(t_visualizer *v)
{
	if (handle_events(v))
		return (0);
	if (sem_lock_no_wait(v->ipc.sem_id) != -1)
	{
		copy_buffer(v->buffer, v->ipc.data, BOARD_HEIGHT);
		sem_unlock(v->ipc.sem_id);
	}
	clear_window(v->renderer, color_from_u32(0xFF000000));
	draw_board(v);
	SDL_RenderPresent(v->renderer);
	if (!is_game_ended(v->buffer))
		return (0);
	return (1);
}

void	sdl_loop(t_visualizer *v)
{
	while (visualizer_routine(v))
		SDL_Delay(16);
	shm_det(v->ipc.data);
	destroy_visualizer(*v);
}

int	visualizer_workflow(void)
{
	t_visualizer	v;

	if (init_visualizer(&v, WIN_TITLE, WIN_WIDTH, WIN_HEIGHT))
		return (1);
	sdl_loop(&v);
	return (0);
}

// void	key_hook(mlx_key_data_t keydata, void *param)
// {
// 	t_visualizer *v = (t_visualizer *) param;
// 	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
// 	{
// 		if (v->running)
// 		{
// 			v->running = 0;
// 			sem_lock(v->ipc.sem_id);
// 		}
// 		else
// 		{
// 			v->running = 1;
// 			sem_unlock(v->ipc.sem_id);
// 		}
// 	}
// }