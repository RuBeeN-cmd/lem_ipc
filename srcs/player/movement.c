#include <player.h>

int	can_move_player(t_game *game, t_vec2 new_pos)
{
	if (new_pos.x >= 0 && new_pos.x < game->board_size.x && new_pos.y >= 0 && new_pos.y < game->board_size.y)
	{
		if (game->board[new_pos.y][new_pos.x] == EMPTY_CELL)
		{
			return (1);
		}
	}
	return (0);
}

int	move_player(t_game *game, t_vec2 new_pos)
{
	if (can_move_player(game, new_pos))
	{
		game->board[game->player.position.y][game->player.position.x] = 0;
		game->player.position = new_pos;
		game->board[new_pos.y][new_pos.x] = game->player.team;
		return (1);
	}
	return (0);
}

int	move_up(t_game *game) {
	t_vec2	new_pos = game->player.position;
	new_pos.y--;
	return move_player(game, new_pos);
}

int	move_down(t_game *game) {
	t_vec2	new_pos = game->player.position;
	new_pos.y++;
	return move_player(game, new_pos);
}

int	move_left(t_game *game) {
	t_vec2	new_pos = game->player.position;
	new_pos.x--;
	return move_player(game, new_pos);
}

int	move_right(t_game *game) {
	t_vec2	new_pos = game->player.position;
	new_pos.x++;
	return move_player(game, new_pos);
}

uint32_t get_lowest_heuristic(uint32_t *heuristics) {
	uint32_t min = UINT32_MAX;
	int idx = -1;
	for (size_t i = 0; i < 4; i++)
	{
		if (heuristics[i] < min) {
			min = heuristics[i];
			idx = i;
		}
	}
	heuristics[idx] = UINT32_MAX;
	return idx;
}

int player_move(t_game *game) {
	t_vec2	target = game->player.target;
	if (!vec2cmp(target, NULL_POS))
		return (1);
	t_vec2 delta = {target.x - game->player.position.x, target.y - game->player.position.y};
	if (ft_abs(delta.x) + ft_abs(delta.y) <= 1)
		return (1);
	t_vec2 base_dir;
	if (ft_abs(delta.x) > ft_abs(delta.y))
		base_dir = (t_vec2) { (delta.x > 0) * 2 - 1, 0 };
	else
		base_dir = (t_vec2) { 0, (delta.y > 0) * 2 - 1 };
	if (move_player(game, add_vec2(game->player.position, base_dir)))
		return (0);
	t_vec2 new_dir = (t_vec2) { base_dir.y, base_dir.x };
	if (move_player(game, add_vec2(game->player.position, new_dir)))
		return (0);
	new_dir = scalar_mult_vec2(new_dir, -1);
	if (move_player(game, add_vec2(game->player.position, new_dir)))
		return (0);
	new_dir = scalar_mult_vec2(base_dir, -1);
	if (move_player(game, add_vec2(game->player.position, new_dir)))
		return (0);
	return (1);
}