#include "lem_ipc.h"

int	player_workflow(uint32_t team)
{
	t_ipc		ipc;
	t_game		game;

	if (init_ipc(&ipc))
		return (1);
	init_game(&game, ipc.data, team);
	if (ipc_join_board(&ipc, &game))
		return (1);
	uint32_t alive = 1;
	uint32_t not_alone = 1;
	usleep(100000);
	while ((alive = is_alive(game, ipc)) && (not_alone = !is_team_alone(game, ipc)))
	{
		sem_lock(ipc.sem_id);
		get_best_move(&game);
		sem_unlock(ipc.sem_id);
		usleep(1000000);
	}
	ft_printf_fd(1, "Team %u is dead\n", team);
	ft_printf_fd(1, "Reason:\n1 Team Remain: %d\nAlive: %d\n", not_alone, alive);
	return 0;
}

int main(int argc, char *argv[])
{
	uint32_t	team;
	
	team = get_team(argc, argv);
	if (team == VISUALIZER)
		return (visualizer_workflow());
	return (player_workflow(team));
}
