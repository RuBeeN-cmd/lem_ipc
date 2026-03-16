#ifndef PLAYER_H
#define PLAYER_H

#include <lem_ipc.h>
#include <ipc.h>

#define COOLDOWN		100000
#define START_COOLDOWN	5000000

#define SCAN_RADIUS		10

typedef struct	s_player {
	t_vec2			position;
	uint32_t		team;
	int				is_supervised;
	t_state			state;
	int				chain_id;
	t_vec2			last_pos;
}				t_player;

typedef struct	s_game
{
	uint32_t		**board;
	t_vec2			board_size;
	uint32_t		chunk_size;
	t_player		player;
}				t_game;

// init_player.c
void	init_player(t_player *player, uint32_t team);

// player.c
int	player_workflow(uint32_t team, t_vec2 board_size);

// game.c
int				get_best_move(t_game *game);
int				is_with_mate(t_game game);
void			go_to_mate(t_game *game);
int				is_killed_by_team(t_game *game);
int				is_other_team(t_game *game, t_ipc *ipc);
t_vec2			rand_pos(t_vec2 board_size);
void			escape_from_enemys(t_game *game);
int 			player_move(t_game *game);
int 			player_move_to_target(t_game *game, t_vec2 target);
int				is_other_mate(t_game game);
int				is_mate_in_chunk(t_game *game, t_vec2 pos);
t_vec2			get_nearest(t_game game, int find_mate);

// init_game.c
void	init_game(t_game *game, t_shm_data *shm_data, uint32_t team, t_vec2 board_size);
int		join_board(t_game *game);
void	init_board(uint32_t **board, uint32_t *raw_board, t_vec2 board_size);

// ipc.c
int	ipc_join_board(t_ipc *ipc, t_game *game);
int	close_ipc(t_ipc *ipc);

// messages.c
void	check_supervision_msg(t_ipc *ipc, t_game *game);
void	send_supervision_info(t_ipc *ipc, t_game *game, int is_alive);
void	send_kill_info(t_ipc *ipc, uint32_t killed_team, uint32_t killer_team);
void	check_leader_position(t_ipc *ipc, t_player *player);
void	send_position(t_ipc *ipc, t_player *player);
void	send_target_position(t_ipc *ipc, t_game *game, t_vec2 target);
void	send_team_msg(t_ipc *ipc, t_game *game);
void	check_team_msg(t_ipc *ipc, t_game *game);

#endif