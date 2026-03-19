#include <player.h>

void	init_player(t_player *player, uint32_t team) {
	player->team = team;
	player->is_supervised = 0;
	player->position = NULL_POS;
	player->target = NULL_POS;
}
