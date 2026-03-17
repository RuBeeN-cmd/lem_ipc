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
		game->player.chain_id,
		game->player.leader.position
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


t_leading_msg	init_leading_msg(t_player *player) {
	t_leading_msg msg = (t_leading_msg) {
		.chain_id = player->chain_id,
		.last_position = player->last_position,
		.position = player->position,
		.target = player->position
	};
	return (msg);
}

void	send_leading_msg(t_ipc *ipc, t_game *game) {
	t_leading_msg msg = init_leading_msg(&game->player);
	DBG("Sending leading message (Chain ID: %d)\n", msg.chain_id);
	send_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	
	game->player.last_position  = game->player.position;
}

void	check_team_msg(t_ipc *ipc, t_game *game) {
	t_leading_msg msg;
	int ret = check_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	if (ret == 1) {
		DBG("Received message in team channel.\n");
		// if (game->player.chain_id == -1) {
		// 	t_vec2 delta = sub_vec2(game->player.position, msg.position);
		// 	if (ft_abs(delta.x) + ft_abs(delta.y) <= SCAN_RADIUS) {
		// 		DBG("Leader message accepted.\n");

		game->player.chain_id = msg.chain_id;
		game->player.leader.position = msg.position;
		game->player.leader.target = msg.target;

		// 		send_leading_msg(ipc, game);
		// 		return ;
		// 	}
		// } else if (msg.chain_id == game->player.chain_id) {
		// 	if (!vec2cmp(msg.last_position, game->player.leader.position)) {
		// 		game->player.chain_id = msg.chain_id;
		// 		game->player.leader.position = msg.position;
		// 		game->player.leader.target = msg.target;

		// 		DBG("Leader message received, passing it.\n");
		// 		send_leading_msg(ipc, game);
		// 	} else if (!vec2cmp(msg.position, game->player.last_position)) {
		// 		DBG("Own message received, recreating a new one.\n");
		// 		send_leading_msg(ipc, game);
		// 	}
		// 	return ;
		// }
		// DBG("Resending it.\n");
		// send_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	}
}