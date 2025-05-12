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
#include <models/log.h>
#include <models/vector.h>

#define PAGE_SIZE		getpagesize()

#define PARENT	0
#define CHILD	1

#define KEY_PATH	"./lemipc"

#define VISUALIZER_CHANNEL			UINT32_MAX
#define PAUSE_CHANNEL				(UINT32_MAX - 1)
#define VISUALIZER_TARGET_CHANNEL	(UINT32_MAX - 2)
#define TARGET_INFOS_CHANNEL		(UINT32_MAX - 3)

typedef struct	s_ipc
{
	int		type;
	key_t	key;
	int		shm_id;
	int		sem_id;
	int		msg_id;
	void	*data;
}				t_ipc;

typedef struct	s_supervised_infos
{
	t_vec2		pos;
	uint32_t	team;
	int			is_alive;
}				t_supervised_infos;

typedef struct 	s_supervised_infos_msg
{
	uint64_t		    type;
	t_supervised_infos	target_infos;
}				t_supervised_infos_msg;

typedef struct 	s_msg
{
	uint64_t	type;
	char		text[8];
}				t_msg;

typedef struct 	s_vec2_msg
{
	uint64_t	type;
	t_vec2		v;
}				t_vec2_msg;

// init_ipc.c
int	init_player_ipc(t_ipc *ipc, t_vec2 board_size);
int	init_visualizer_ipc(t_ipc *ipc, t_vec2 board_size);

// message.c
int	check_target_infos_msg(int msg_id, t_supervised_infos *target_infos);
int	send_target_infos_msg(int msg_id, t_supervised_infos target_infos);
int	send_visualizer_target_msg(int msg_id, t_vec2 target);
int	check_visualizer_target_msg(int msg_id, t_vec2 *target);
int	send_pause_msg(int msg_id);
int	check_pause_msg(int msg_id);

// ipc_utils.c
uint32_t	get_shm_size(t_vec2 board_size);
void		sem_lock(int sem_id);
int			sem_lock_no_wait(int sem_id);
void		sem_unlock(int sem_id);
int			sem_destroy(int sem_id);
int			get_nb_process_attach(int shm_id);
int			shm_det(void *data);
int			shm_destroy(int shm_id);
int			msg_queue_destroy(int msg_id);

#endif