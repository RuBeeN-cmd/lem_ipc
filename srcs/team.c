#include "lem_ipc.h"

uint32_t	get_team_from_str(const char *nptr)
{
	unsigned int	nb;
	char			*str;

	nb = 0;
	str = (char *) nptr;
	while (*str >= '0' && *str <= '9')
	{
		if (nb > (nb + *str - '0') * 10)
			return (0);
		nb *= 10;
		nb += *str++ - '0';
	}
	if (*str)
		return (0);
	return (nb);
}

uint32_t	get_team(int argc, char *argv[])
{
	if (argc != 2)
	{
		ft_printf_fd(2, "Usage: %s <team id>\n", argv[0]);
		exit(1);
	}
	if (!ft_strncmp(argv[1], "visualizer", 11))
		return (VISUALIZER);
	uint32_t team = get_team_from_str(argv[1]);
	if (team < 1 || team > MAX_TEAMS)
	{
		ft_log(LOG_ERROR, "Invalid team id\n");
		exit(1);
	}
	return (team);
}
