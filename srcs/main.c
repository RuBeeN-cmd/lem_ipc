#include <lem_ipc.h>
#include <visualizer/visualizer.h>
#include <player.h>

int	parse_args(int argc, char *argv[], uint32_t *team, t_vec2 *board_size)
{
	if (argc < 2 || argc > 4)
	{
		ft_printf_fd(2, "Usage: %s <team id> [board-width] [board-height]\n", argv[0]);
		return (1);
	}
	*team = get_team(argv[1]);
	if (!*team)
	{
		ft_log(LOG_ERROR, "Invalid team id\n");
		return (1);
	}
	if (argc != 2 && *team == VISUALIZER)
		ft_log(LOG_WARNING, "You can't specify board size for the visualizer. Ignoring it.\n");
	else if (argc == 3)
		ft_log(LOG_WARNING, "You must specify both board width and height. Ignoring it.\n");
	*board_size = DEF_BOARD_SIZE;
	if (argc == 4)
	{
		board_size->x = ft_atoi(argv[2]);
		board_size->y = ft_atoi(argv[3]);
	}
	return (0);
}

int main(int argc, char *argv[])
{
	uint32_t	team;
	t_vec2		board_size;
	
	parse_args(argc, argv, &team, &board_size);
	if (team == VISUALIZER)
		return (visualizer_workflow());
	return (player_workflow(team, board_size));
}
