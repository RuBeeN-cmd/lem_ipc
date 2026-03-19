#include <player.h>

void	check_supervision_msg(t_ipc *ipc, t_game *game)
{
	t_new_target_msg visualizer_target;
	int ret = check_msg(ipc->msg_id, &visualizer_target, sizeof(visualizer_target), VISUALIZER_TARGET_CHANNEL);
	if (ret == 1) {
		if (!vec2cmp(visualizer_target.target, game->player.position)) {
			if (visualizer_target.type == STOP_TARGETING)
				DBG("Player catch: STOP TARGET\n");
			else
				DBG("Player catch: NEW TARGET\n");
			game->player.is_supervised = visualizer_target.type;
		}
		else
			send_msg(ipc->msg_id, &visualizer_target, sizeof(visualizer_target), VISUALIZER_TARGET_CHANNEL);
	}
}

void	send_supervision_info(t_ipc *ipc, t_game *game, int is_alive)
{
	t_supervised_infos target_infos = {
		game->player.position,
		game->player.team,
		is_alive,
		game->player.targeted_team,
		game->player.target
	};
	send_msg(ipc->msg_id, &target_infos, sizeof(t_supervised_infos), TARGET_INFOS_CHANNEL);
}

void	send_kill_info(t_ipc *ipc, uint32_t killed_team, uint32_t killer_team)
{
	t_kill_info	kill_info;
	kill_info.killed_team = killed_team;
	kill_info.killer_team = killer_team;
	DBG("Player sending kill info: killed_team=%u, killer_team=%u\n", killed_team, killer_team);
	send_msg(ipc->msg_id, &kill_info, sizeof(t_kill_info), KILL_CHANNEL);
}

void	choose_target_team(t_ipc *ipc, t_game *game)
{
	(void) ipc;
	t_vec2		nearest_enemy = get_nearest(game, 0);
	DBG("Nearest Enemy: (%d; %d)\n", nearest_enemy.x, nearest_enemy.y);
	if (!vec2cmp(nearest_enemy, NULL_POS))
		return ;
	uint32_t	targeted_team = game->board[nearest_enemy.y][nearest_enemy.x];
	if (targeted_team) {
		send_msg(ipc->msg_id, &targeted_team, sizeof(targeted_team), game->player.team);
		game->player.targeted_team = targeted_team;
		DBG("New Target team Choosed\n");
	}
}

int	get_target_team(t_ipc *ipc, t_game *game)
{
	uint32_t targeted_team;
	int ret = check_msg(ipc->msg_id, &targeted_team, sizeof(targeted_team), game->player.team);
	if (ret == 1) {
		if (!targeted_team)
			return (0);
		game->player.targeted_team = targeted_team;
		send_msg(ipc->msg_id, &targeted_team, sizeof(targeted_team), game->player.team);
		DBG("Target team Updated: %d\n", targeted_team);
		return (1);
	}
	return (0);
}