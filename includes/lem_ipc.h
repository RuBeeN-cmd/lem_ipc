#ifndef LEM_IPC_H
#define LEM_IPC_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <libft.h>
#include <stdio.h>
#include <MLX42.h>

#define WIN_WIDTH	800
#define WIN_HEIGHT	600
#define CELL_SIZE	30
#define MOVE_SPEED	1
#define ZOOM_SPEED	1


#define PARENT		0
#define CHILD		1

#define MAX_TEAMS	UINT32_MAX - 1
#define VISUALIZER	UINT32_MAX

#define BOARD_WIDTH		11
#define BOARD_HEIGHT	11

#define PAGE_SIZE		getpagesize()

#define C_RED	"\x1b[31m"
#define C_GRN	"\x1b[32m"
#define C_YEL	"\x1b[33m"
#define C_BLU	"\x1b[34m"
#define C_MAG	"\x1b[35m"
#define C_CYN	"\x1b[36m"
#define C_WHT	"\x1b[37m"
#define C_END	"\x1b[0m"

typedef struct	s_vec2
{
	int		x;
	int		y;
}				t_vec2;

typedef struct	s_game
{
	int			*board[BOARD_HEIGHT];
	t_vec2		position;
	uint32_t	team;
}				t_game;

typedef struct	s_ipc
{
	key_t	key;
	int		sem_id;
	void	*data;
	int		type;
}				t_ipc;

typedef struct	s_visualizer
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_vec2		offset;
	float		scale;
	int			*board[BOARD_HEIGHT];
	t_ipc		ipc;
	int			running;
}				t_visualizer;

// ipc.c
int		init_ipc(t_ipc *ipc);
void	sem_unlock(int sem_id);
void	sem_lock(int sem_id);
int		sem_lock_no_wait(int sem_id);
void	ipc_join_board(t_ipc *ipc, t_game *game);
int		*get_shm_data(key_t key, int flags);

// game.c
void	init_game(t_game *game, int *raw_board, int team);
void	move_random(t_game *game);

// board.c
void	init_board(int **board, int *raw_board);
void	show_board(int **board);
size_t	get_board_size_padded(void);
void	join_board(t_game *game);

// visualizer.c
int		init_visualizer(t_visualizer *visualizer);
void	render(void *param);
void	handle_keys(t_visualizer* visualizer);
void	key_hook(mlx_key_data_t keydata, void *param);

// vec2.c
t_vec2	init_vec2(int x, int y);

#endif