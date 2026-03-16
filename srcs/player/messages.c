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
		game->player.state,
		game->player.chain_id
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

void	send_team_msg(t_ipc *ipc, t_game *game) {
	DBG("Sending Chain Request (Chain ID: %d)\n", game->player.chain_id);
	t_team_msg msg;
	msg.chain_id = game->player.chain_id;
	msg.last_pos = NULL_POS;
	msg.pos = game->player.position;
	send_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
}

void	check_team_msg(t_ipc *ipc, t_game *game) {
	t_team_msg msg;
	int ret = check_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	if (ret == 1) {
		DBG("Team msg rcv.\n");
		if (game->player.state == UNPAIRED) {
			t_vec2 delta = sub_vec2(game->player.position, msg.pos);
			if (ft_abs(delta.x) + ft_abs(delta.y) <= SCAN_RADIUS) {
				game->player.state = LEADING;
				game->player.chain_id = msg.chain_id;
				game->player.last_pos = game->player.position;
				DBG("Message found, became Leader too\n");
				return ;
			}
		}
		send_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	}
}