#include "lem_ipc.h"

int	player_workflow(uint32_t team)
{
	t_ipc	ipc;
	t_game	game;

	if (init_ipc(&ipc))
		return (1);
	init_game(&game, ipc.data, team);
	if (ipc_join_board(&ipc, &game))
		return (1);
	#ifdef OSX
		usleep(100000); // for macos: to prevent lock order
	#endif

	while (is_alive(game, ipc) && is_other_team(game, ipc))
	{
		sem_lock(ipc.sem_id);

		if (check_pause_msg(ipc) == 1)
		{
			send_pause_msg(ipc);
			sem_unlock(ipc.sem_id);
			usleep(COOLDOWN);
			continue ;
		}
		if (!is_with_mate(game)) {
			go_to_mate(&game);
		} else {
			get_best_move(&game);
		}
		sem_unlock(ipc.sem_id);
		usleep(COOLDOWN);
	}
	close_ipc(ipc);
	return 0;
}

int main(int argc, char *argv[])
{
	uint32_t	team;
	
	team = get_team(argc, argv);
	if (team == VISUALIZER_CHANNEL)
		return (visualizer_workflow());
	return (player_workflow(team));
}
