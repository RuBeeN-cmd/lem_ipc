#include "lem_ipc.h"

int main(int argc, char *argv[])
{
	uint32_t	team;
	
	team = get_team(argc, argv);
	if (team == VISUALIZER)
		return (visualizer_workflow());
	return (player_workflow(team));
}
