#ifndef PLAYER_H
#define PLAYER_H

#include <lem_ipc.h>
#include <ipc.h>

#define COOLDOWN		100000
#define START_COOLDOWN	5

typedef struct	s_game
{
	uint32_t	**board;
	t_vec2		board_size;
	t_vec2		position;
	uint32_t	team;
	int			is_supervised;
}				t_game;

// player.c
int	player_workflow(uint32_t team, t_vec2 board_size);

// game.c
int		get_best_move(t_game *game);
int		is_with_mate(t_game game);
void	go_to_mate(t_game *game);
int		is_killed_by_team(t_game *game, t_ipc *ipc);
int		is_other_team(t_game *game, t_ipc *ipc);
t_vec2	rand_pos(t_vec2 board_size);
int		is_game_paused(t_ipc *ipc);
void	escape_from_enemys(t_game *game);
void	player_routine(t_game *game);
int		is_other_mate(t_game game);

// init_game.c
void	init_game(t_game *game, uint32_t *raw_board, uint32_t team, t_vec2 board_size);
int		join_board(t_game *game);
void	init_board(uint32_t **board, uint32_t *raw_board, t_vec2 board_size);

// ipc.c
int	ipc_join_board(t_ipc *ipc, t_game *game);
int	close_ipc(t_ipc *ipc);

// messages.c
void	check_supervision_msg(t_ipc *ipc, t_game *game);
void	send_supervision_info(t_ipc *ipc, t_game *game, int is_alive);
void	send_kill_info(t_ipc *ipc, uint32_t killed_team, uint32_t killer_team);

#endif