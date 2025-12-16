#ifndef IPC_H
#define IPC_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
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


typedef struct	s_ipc
{
	int		type;
	key_t	key;
	int		shm_id;
	int		sem_id;
	int		msg_id;
	void	*data;
	key_t	init_key;
	int		init_sem_id;
	t_vec2	board_size;
}				t_ipc;

typedef struct	s_supervised_infos
{
	t_vec2		pos;
	uint32_t	team;
	int			is_alive;
}				t_supervised_infos;

typedef struct	s_supervised_infos_msg
{
	uint64_t		    type;
	t_supervised_infos	target_infos;
}				t_supervised_infos_msg;

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
t_new_target_msg	new_msg(t_vec2 target, t_new_target_msg_type type);

// ipc_utils.c
uint32_t	get_shm_size(t_vec2 board_size);
int			sem_lock(int sem_id);
int			sem_lock_no_wait(int sem_id);
int			sem_unlock(int sem_id);
int			sem_destroy(int sem_id);
int			get_nb_process_attach(int shm_id);
int			shm_det(void *data);
int			shm_destroy(int shm_id);
int			msg_queue_destroy(int msg_id);

#endif