#ifndef PLAYER_H
#define PLAYER_H

#include <lem_ipc.h>
#include <ipc.h>

#define COOLDOWN		100000

typedef struct	s_game
{
	uint32_t	*board[BOARD_HEIGHT];
	t_vec2		position;
	uint32_t	team;
	int			is_supervised;
}				t_game;

// player.c
int	player_workflow(uint32_t team);

// game.c
int		get_best_move(t_game *game);
int		is_with_mate(t_game game);
void	go_to_mate(t_game *game);
int		is_alive(t_game *game, t_ipc *ipc);
int		is_other_team(t_game *game, t_ipc *ipc);
t_vec2	rand_pos(void);
int		is_game_paused(t_ipc *ipc);

// init_game.c
void	init_game(t_game *game, uint32_t *raw_board, uint32_t team);
int		join_board(t_game *game);
void	init_board(uint32_t **board, uint32_t *raw_board);

// ipc.c
int	ipc_join_board(t_ipc *ipc, t_game *game);
int	close_ipc(t_ipc *ipc);

#endif