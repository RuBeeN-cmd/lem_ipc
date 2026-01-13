#include <visualizer/visualizer.h>

static int	is_on_board(t_vec2 pos, t_vec2 board_size)
{
	return (pos.x >= 0 && pos.x < board_size.x
		&& pos.y >= 0 && pos.y < board_size.y);
}

static t_vec2	screen_pos_to_board_pos(t_visualizer *v, t_vec2 screen_pos)
{
	t_vec2 renderer_size;
	SDL_GetCurrentRenderOutputSize(v->renderer, &(renderer_size.x), &(renderer_size.y));

	t_vec2	v1 = sub_vec2(screen_pos, scalar_div_vec2(renderer_size, 2));
	t_fvec2	v2 = scalar_div_fvec2(vec2_to_fvec2(v1), v->cell_size);
	v2 = add_fvec2(v2, v->offset);
	v2 = add_fvec2(v2, scalar_div_fvec2(vec2_to_fvec2(v->board_size), 2));
	v1 = fvec2_to_vec2(v2);
	if (is_on_board(v1, v->board_size))
		return (v1);
	return (NULL_POS);
}

static void	zoom(t_visualizer *v, float zoom_factor)
{
	uint32_t old_cell_size = v->cell_size;
	v->cell_size *= zoom_factor;
	if (old_cell_size == v->cell_size)
	{
		if (zoom_factor < 1)
			v->cell_size--;
		else
			v->cell_size++;
	}
	v->cell_size = ft_max(v->cell_size, 4);
	v->cell_size = ft_min(v->cell_size, 100);
}

static void	move(t_visualizer *v, t_vec2 offset)
{
	v->offset = add_fvec2(v->offset, scalar_div_fvec2(vec2_to_fvec2(offset), v->cell_size));
}

static void	toggle_pause(t_visualizer *v)
{
	sem_lock(v->ipc.sem_id);
	DBG("Pause toggled\n");
	v->ipc.data->game_state ^= PAUSED;
	sem_unlock(v->ipc.sem_id);
}

static int	on_key_down(SDL_Keycode key, t_visualizer *v)
{
	if (key == SDLK_ESCAPE)
		return (1);
	else if (key == SDLK_UP)
		zoom(v, 1.1);
	else if (key == SDLK_DOWN)
		zoom(v, 1 / 1.1);
	else if (key == SDLK_W)
		move(v, (t_vec2) {0, -MOVE_SPEED});
	else if (key == SDLK_A)
		move(v, (t_vec2) {-MOVE_SPEED, 0});
	else if (key == SDLK_S)
		move(v, (t_vec2) {0, MOVE_SPEED});
	else if (key == SDLK_D)
		move(v, (t_vec2) {MOVE_SPEED, 0});
	else if (key == SDLK_SPACE)
		toggle_pause(v);
	return (0);
}

static void	stop_supervising(t_visualizer *v, t_vec2 supervised)
{
	if (vec2cmp(supervised, NULL_POS))
	{
		t_new_target_msg erase_target_msg = new_msg(supervised, STOP_TARGETING);
		send_msg(v->ipc.msg_id, &erase_target_msg, sizeof(erase_target_msg), VISUALIZER_TARGET_CHANNEL);
		v->supervision_panel.visible = 0;
	}
}

static void	supervise(t_visualizer *v, t_vec2 supervised)
{
	if (vec2cmp(supervised, NULL_POS))
	{
		t_new_target_msg new_target_msg = new_msg(supervised, NEW_TARGETING);
		send_msg(v->ipc.msg_id, &new_target_msg, sizeof(new_target_msg), VISUALIZER_TARGET_CHANNEL);
		v->supervision_panel.visible = 1;
	}
}

static void	on_click(t_visualizer *v, t_vec2 pos)
{
	t_new_target_msg new_target_msg;
	new_target_msg.type = NEW_TARGETING;
	new_target_msg.target = screen_pos_to_board_pos(v, (t_vec2) {pos.x, pos.y});
	
	t_vec2	click_pos = screen_pos_to_board_pos(v, pos);
	if (vec2cmp(click_pos, NULL_POS))
	{
		stop_supervising(v, v->target_infos.pos);
		t_new_target_msg new_target_msg = new_msg(click_pos, NEW_TARGETING);
		sem_lock(v->ipc.sem_id);
		if (get_team_on_board(new_target_msg.target, v->buffer, v->board_size)) {
			sem_unlock(v->ipc.sem_id);
			supervise(v, new_target_msg.target);
			DBG("Supervision msg sent\n");
		} else {
			sem_unlock(v->ipc.sem_id);
			v->target_infos.pos = NULL_POS;
		}
	}
}

int	handle_events(t_visualizer* v)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
			return (1);
		else if (e.type == SDL_EVENT_KEY_DOWN) {
			if (on_key_down(e.key.key, v))
			return (1);
		}
		else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
			on_click(v, (t_vec2) {e.button.x, e.button.y});
	}
	return (0);
}