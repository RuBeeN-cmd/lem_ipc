#include <lem_ipc.h>
#include <visualizer/visualizer.h>
#include <player.h>

int main(int argc, char *argv[])
{
	uint32_t	team;
	t_vec2		board_size;
	
	if (parse_args(argc, argv, &team, &board_size))
		return (1);
	if (team == VISUALIZER)
		return (visualizer_workflow());
	return (player_workflow(team, board_size));
}
