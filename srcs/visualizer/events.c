#include "lem_ipc.h"

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
				sem_lock(v->ipc.sem_id);
				if (v->running == 1) {
					v->running = 0;
					ft_log(LOG_DEBUG, "Paused\n");
					send_pause_msg(v->ipc.msg_id);
				} else if (v->running == 0) {
					v->running = 1;
					ft_log(LOG_DEBUG, "Unpaused\n");
					check_pause_msg(v->ipc.msg_id);
				}
				sem_unlock(v->ipc.sem_id);
			}
		} else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				t_vec2	target = screen_pos_to_board_pos(v, (t_vec2) {e.button.x, e.button.y});
				if (vec2cmp(target, NULL_POS))
				{
					ft_printf_fd(1, "Clicked on : (%d, %d)\n", target.x, target.y);
					send_visualizer_target_msg(v->ipc.msg_id, target);	
				}
			}
		}
	}
	return (0);
}