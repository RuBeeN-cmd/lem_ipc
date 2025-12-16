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
	DBG("Parsing arguments\n");
	if (argc < 2 || argc > 4)
	{
		ERR("Usage: %s <team id> [board-width] [board-height]\n", argv[0]);
		return (1);
	}
	*team = get_team(argv[1]);
	if (!*team)
	{
		ERR("Invalid team id.\n");
		return (1);
	}
	*board_size = DEF_BOARD_SIZE;
	if (*team == VISUALIZER)
	{
		if (argc != 2) 
			WARN("You can't specify board size for the visualizer. Ignoring it.\n");
	}
	else
	{
		if (argc >= 3)
		{	
			if (!str_is_digit(argv[2]))
			{
				ERR("Invalid board width.\n");
				return (1);
			}
			board_size->x = ft_atoi(argv[2]);
			if (board_size->x <= 0)
			{
				ERR("Invalid board width.\n");
				return (1);
			}
		}
		if (argc == 4)
		{
			if (!str_is_digit(argv[3]))
			{
				ERR("Invalid board height.\n");
				return (1);
			}
			board_size->y = ft_atoi(argv[3]);
			if (board_size->y <= 0)
			{
				ERR("Invalid board height.\n");
				return (1);
			}
		}
		if (argc == 3)
		{
			WARN("You must specify both board width and height. Ignoring it.\n");
			*board_size = DEF_BOARD_SIZE;
		}
	}
	return (0);
}