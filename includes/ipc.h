#ifndef IPC_H
#define IPC_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <libft.h>
#include <utils/log.h>
#include <models/vector.h>

#define PAGE_SIZE		getpagesize()

#define PARENT	0
#define CHILD	1

#define KEY_PATH		"./lemipc"
#define INIT_PROJ_ID	'L'
#define SHM_PROJ_ID		'R'

#define VISUALIZER_CHANNEL			UINT32_MAX
#define PAUSE_CHANNEL				(UINT32_MAX - 1)
#define VISUALIZER_TARGET_CHANNEL	(UINT32_MAX - 2)
#define TARGET_INFOS_CHANNEL		(UINT32_MAX - 3)
#define BOARD_SIZE_CHANNEL			(UINT32_MAX - 4)
#define KILL_CHANNEL				(UINT32_MAX - 5)
#define GAME_START_CHANNEL			(UINT32_MAX - 6)

#define SHM_DATA_SIZE(board_size)	(sizeof(t_shm_data) + ((board_size).x * (board_size).y * sizeof(uint32_t)))
#define SHM_BOARD_OFFSET			(sizeof(t_game_state))

typedef enum e_game_state
{
	STARTED = 1,
	PAUSED = 2
}				t_game_state;

typedef struct	s_shm_data
{
	t_game_state	game_state;
}				t_shm_data;

typedef struct	s_ipc
{
	int			type;
	key_t		key;
	int			shm_id;
	int			sem_id;
	int			msg_id;
	key_t		init_key;
	int			init_sem_id;
	t_shm_data	*data;
}				t_ipc;

typedef struct	s_supervised_infos
{
	t_vec2		pos;
	uint32_t	team;
	int			is_alive;
}				t_supervised_infos;

typedef struct	s_kill_info
{
	uint32_t	killed_team;
	uint32_t	killer_team;
}				t_kill_info;

typedef struct	s_kill_number
{
	uint32_t	team;
	uint32_t	number;
}				t_kill_number;

typedef enum	e_new_target_msg_type
{
	STOP_TARGETING,
	NEW_TARGETING
}				t_new_target_msg_type;

typedef struct	s_new_target_msg
{
	t_new_target_msg_type	type;
	t_vec2					target;
}				t_new_target_msg;

typedef struct	s_vec2_msg
{
	uint64_t	type;
	t_vec2		vec2;
}				t_vec2_msg;

// init_ipc.c
int	init_player_ipc(t_ipc *ipc, t_vec2 *board_size);
int	init_visualizer_ipc(t_ipc *ipc, t_vec2 *board_size);

// message.c
int					check_msg(int msg_id, void *data, uint32_t data_size, uint32_t channel);
int					send_msg(int msg_id, void *data, uint32_t data_size, uint32_t channel);
t_new_target_msg	new_target_msg(t_vec2 target, t_new_target_msg_type type);
void				clean_msg_queue(t_ipc *ipc);
int					msgq_destroy(int msg_id);

// ipc_utils.c
uint32_t	get_shm_size(t_vec2 board_size);
int			sem_lock(int sem_id);
int			sem_lock_no_wait(int sem_id);
int			sem_unlock(int sem_id);
int			sem_destroy(int sem_id);
int			get_attached_process_nb(int shm_id);
uint32_t	get_msgq_size(int msgid);

// shm_utils.c
int				shm_detach(void *data);
int				shm_destroy(int shm_id);

#endif