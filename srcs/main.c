#include "lem_ipc.h"

unsigned int	get_team_from_str(const char *nptr)
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
		ft_printf_fd(2, "Invalid team id\n");
		exit(1);
	}
	return (team);
}

#include <time.h>

int main(int argc, char *argv[])
{
	t_ipc	ipc;
	t_game	game;
	
	srand(time(NULL));
	uint32_t	team = get_team(argc, argv);
	if (team == VISUALIZER)
	{
		t_visualizer visualizer;
		if (init_visualizer(&visualizer))
			return 1;
		mlx_loop(visualizer.mlx);
		mlx_terminate(visualizer.mlx);
		return 0;
	}
	if (init_ipc(&ipc))
		return (1);
	init_game(&game, ipc.data, team);
	ipc_join_board(&ipc, &game);
	for (size_t i = 0; i < 3; i++)
	{
		sem_lock(ipc.sem_id);
		move_random(&game);
		sem_unlock(ipc.sem_id);
		usleep(100000);
	}
	return 0;
}
