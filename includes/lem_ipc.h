#ifndef LEM_IPC_H
#define LEM_IPC_H

#include <stdint.h>
#include <libft.h>
#include <models/vector.h>
#include <utils/log.h>
#include <models/color.h>

#define DEF_BOARD_WIDTH		30
#define DEF_BOARD_HEIGHT	30
#define DEF_BOARD_SIZE		((t_vec2) {DEF_BOARD_WIDTH, DEF_BOARD_HEIGHT})

#define NULL_POS		((t_vec2) {-1, -1})
#define EMPTY_CELL		0

#define MAX_TEAMS		(UINT32_MAX - 10)
#define VISUALIZER		UINT32_MAX

// args.c
int	parse_args(int argc, char *argv[], uint32_t *team, t_vec2 *board_size);

// team.c
uint32_t	get_team(char *team_str);
uint32_t	get_team_on_board(t_vec2 target, uint32_t *board[], t_vec2 board_size);
t_color		get_team_color(int team);
t_color		hsl2rgb(uint16_t h_deg, float s, float l);

#endif