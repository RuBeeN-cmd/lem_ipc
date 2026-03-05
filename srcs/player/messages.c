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
		game->player.is_chunk_leader,
		game->player.leader.pos
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

void	send_leading_msg(t_ipc *ipc, t_game *game) {
	static int sent = 0;
	t_leader_info	msg = (t_leader_info) { game->pid, game->player.position };
	if (!sent)
		send_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	sent = 1;
}

int	is_same_chunk(uint32_t chunk_size, t_vec2 pos1, t_vec2 pos2) {
	return (!vec2cmp(scalar_div_vec2(pos1, chunk_size), scalar_div_vec2(pos2, chunk_size)));
}

void	get_leading_msg(t_ipc *ipc, t_game *game) {
	t_leader_info msg;
	int ret = check_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	if (ret == 1) {
		if (!game->player.leader.pid) {
			if (is_same_chunk(game->chunk_size, msg.pos, game->player.position)) {
				game->player.leader.pid = msg.pid;
				game->player.leader.pos = msg.pos;
			}
		} else if (msg.pid == game->player.leader.pid) {
			game->player.leader.pos = msg.pos;
		}
		send_msg(ipc->msg_id, &msg, sizeof(msg), game->player.team);
	}
}

void check_team_msg(t_ipc *ipc, t_game *game) {
	if (game->player.is_chunk_leader) {
		if (is_mate_in_chunk(game, game->player.position))
			send_leading_msg(ipc, game);
		else
			game->player.is_chunk_leader = 0;
	} else {
		get_leading_msg(ipc, game);
	}
}