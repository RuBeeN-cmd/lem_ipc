#ifndef LEM_IPC_H
#define LEM_IPC_H

#include <stdint.h>
#include <libft.h>
#include <utils/vector.h>
#include <utils/log.h>

#define BOARD_WIDTH		30
#define BOARD_HEIGHT	30
#define BOARD_SIZE		((t_vec2) {BOARD_WIDTH, BOARD_HEIGHT})

#define NULL_POS		((t_vec2) {-1, -1})
#define EMPTY_CELL		0

#define MAX_TEAMS		(UINT32_MAX - 10)
#define VISUALIZER		UINT32_MAX

// team.c
uint32_t	get_team(int argc, char *argv[]);
uint32_t	get_team_on_board(t_vec2 target, uint32_t *board[]);

#endif