#ifndef LEM_IPC_H
#define LEM_IPC_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <libft.h>
#include <stdio.h>
#include <MLX42.h>
#include <sys/msg.h>
#include <errno.h>

#define C_END	"\033[0m"
#define C_RED	"\033[0;31m"
#define C_GRN	"\033[0;32m"
#define C_YEL	"\033[0;33m"
#define C_BLU	"\033[0;34m"
#define C_MAG	"\033[0;35m"
#define C_CYN	"\033[0;36m"
#define C_WHT	"\033[0;37m"

#define KEY_PATH	"./lemipc"

#define WIN_WIDTH	800
#define WIN_HEIGHT	600
#define CELL_SIZE	30
#define MOVE_SPEED	1
#define ZOOM_SPEED	1
#define COOLDOWN	1000000

#define PARENT		0
#define CHILD		1

#define MAX_TEAMS	(UINT32_MAX - 1)
#define VISUALIZER	UINT32_MAX

#define BOARD_WIDTH		11
#define BOARD_HEIGHT	11

#define PAGE_SIZE		getpagesize()

#define UP_LEFT_MOVE		0
#define UP_MOVE				1
#define UP_RIGHT_MOVE		2
#define RIGHT_MOVE			3
#define BOTTOM_RIGHT_MOVE	4
#define BOTTOM_MOVE			5
#define BOTTOM_LEFT_MOVE	6
#define LEFT_MOVE			7

typedef struct	s_vec2
{
	int		x;
	int		y;
}				t_vec2;

typedef struct	s_game
{
	uint32_t	*board[BOARD_HEIGHT];
	t_vec2		position;
	uint32_t	team;
}				t_game;

typedef struct	s_ipc
{
	int		type;
	key_t	key;
	int		shm_id;
	int		sem_id;
	int		msg_id;
	void	*data;
}				t_ipc;

typedef struct	s_visualizer
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_vec2		offset;
	float		scale;
	uint32_t	**buffer;
	t_ipc		ipc;
	int			running;
}				t_visualizer;
 
typedef struct 	s_msg
{
	long	type;
	char	text[1024 - 8];
}				t_msg;

#define LOG_DEBUG	0
#define LOG_INFO	1
#define LOG_WARNING	2
#define LOG_ERROR	3

// init_ipc.c
int		init_ipc(t_ipc *ipc);

// init_game.c
void	init_game(t_game *game, uint32_t *raw_board, uint32_t team);
int		join_board(t_game *game);










// ipc.c
int		ipc_join_board(t_ipc *ipc, t_game *game);
int		get_shm_id(key_t key, int flags);
int		*get_shm_data(int shm_id);
int		close_ipc(t_ipc ipc);
int		init_msg_queue(key_t key);
int		is_visualizer(t_ipc ipc);

// ipc_utils.c
void	sem_unlock(int sem_id);
void	sem_lock(int sem_id);
int		sem_lock_no_wait(int sem_id);
int		sem_destroy(int sem_id);
int		get_nb_process_attach(int shm_id);
int		shm_det(void *data);
int		shm_destroy(int shm_id);
int		msg_queue_destroy(int msg_id);

// game.c
int		get_best_move(t_game *game);
int		is_alive(t_game game, t_ipc ipc);
int		is_other_team(t_game game, t_ipc ipc);
t_vec2	rand_pos(void);

// board.c
void	init_board(uint32_t **board, uint32_t *raw_board);
size_t	get_board_size_padded(void);

// visualizer.c
int		visualizer_workflow(void);
int		init_visualizer(t_visualizer *v);
void	visualizer_loop(void *param);
void	handle_keys(t_visualizer* v);
void	key_hook(mlx_key_data_t keydata, void *param);
void	free_buffer(uint32_t **buffer, size_t height);

// visualizer_ipc.c
int	init_visualizer_ipc(t_ipc *ipc);

// buffer.c
int		init_buffer(uint32_t ***buffer);
void	free_buffer(uint32_t **buffer, size_t height);
void	copy_buffer(uint32_t **dst, uint32_t *src, size_t height);

// draw.c
void	draw_board(t_visualizer *v);
void	clear_window(mlx_image_t *img, int color);

// vec2.c
t_vec2	init_vec2(int x, int y);

// team.c
uint32_t	get_team(int argc, char *argv[]);

// log.c
void	ft_log(int level, char *msg);

#endif