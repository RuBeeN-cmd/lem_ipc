#include <lem_ipc.h>

static int	str_is_digit(const char *str)
{
	for (size_t i = 0; str[i]; i++)
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}	

int	parse_args(int argc, char *argv[], uint32_t *team, t_vec2 *board_size)
{
	if (argc < 2 || argc > 4)
	{
		ERR("Usage: %s <team id> [board-width] [board-height]", argv[0]);
		return (1);
	}
	*team = get_team(argv[1]);
	if (!*team)
	{
		ERR("Invalid team id.");
		return (1);
	}
	*board_size = DEF_BOARD_SIZE;
	if (*team == VISUALIZER)
	{
		if (argc != 2) 
			WARN("You can't specify board size for the visualizer. Ignoring it.");
	}
	else
	{
		if (argc >= 3)
		{	
			if (!str_is_digit(argv[2]))
			{
				ERR("Invalid board width.");
				return (1);
			}
			board_size->x = ft_atoi(argv[2]);
		}
		if (argc == 4)
		{
			if (!str_is_digit(argv[3]))
			{
				ERR("Invalid board height.");
				return (1);
			}
			board_size->y = ft_atoi(argv[3]);
		}
		if (argc == 3)
		{
			WARN("You must specify both board width and height. Ignoring it.");
			*board_size = DEF_BOARD_SIZE;
		}	
	}
	return (0);
}