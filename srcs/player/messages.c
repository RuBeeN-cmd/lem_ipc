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
		game->player.is_leader,
		game->player.leader
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

void	check_leader_position(t_ipc *ipc, t_player *player) {
	t_leader_msg msg;
	int ret = check_msg(ipc->msg_id, &msg, sizeof(msg), player->team);
	if (ret == 1) {
		t_vec2 delta = sub_vec2(player->position, msg.pos);
		uint32_t dist = ft_abs(delta.x) + ft_abs(delta.y);
		if (dist < LEADER_DIST_THRESHOLD) {
			t_vec2 crt_delta = sub_vec2(player->position, player->leader);
			uint32_t crt_dist = ft_abs(crt_delta.x) + ft_abs(crt_delta.y);
			if (dist < crt_dist) {
				player->leader = msg.pos;
				DBG("Position received\n");
			}
		}
		msg.ttl--;
		DBG("Message TTL: %d\n", msg.ttl);
		if (msg.ttl) {
			send_msg(ipc->msg_id, &msg, sizeof(msg), player->team);
			DBG("Position Msg forwarded\n");
		} else {
			DBG("Position Msg destroyed\n");
		}
	}
}

void	send_position(t_ipc *ipc, t_player *player) {
	t_leader_msg	msg = (t_leader_msg) { player->position, LEADER_MSG_TTL };
	send_msg(ipc->msg_id, &msg, sizeof(msg), player->team);
	DBG("Position sent\n");
}

void	send_target_position(t_ipc *ipc, t_game *game, t_vec2 target) {
	t_leader_msg	msg = (t_leader_msg) { target, LEADER_MSG_TTL };
	send_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	DBG("Position sent\n");
}