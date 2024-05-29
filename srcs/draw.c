#include "lem_ipc.h"

uint32_t	get_team_color(int team)
{
	int		team_color = 0;
	switch (team)
	{
		case 1:
			team_color = 0xFFFF0000;
			break;
		case 2:
			team_color = 0xFF00FF00;
			break;
		case 3:
			team_color = 0xFF0000FF;
			break;
		default:
			team_color = 0xFFFFFFFF;
			break;
	}
	return (team_color);
}

void	draw_cell(t_visualizer *visualizer, int x, int y, int team)
{
	uint32_t	team_color = get_team_color(team);
	size_t		cell_size = visualizer->scale * CELL_SIZE;
	t_vec2		pos = init_vec2(visualizer->offset.x + (x - BOARD_WIDTH / 2.0) * cell_size, visualizer->offset.y + (y - BOARD_HEIGHT / 2.0) * cell_size);
	for (size_t i = 0; i < cell_size; i++)
	{
		for (size_t j = 0; j < cell_size; j++)
		{
			int color = team_color; 
			if (i == cell_size - 1 || j == cell_size - 1)
				color = 0xFF000000;
			if (pos.x >= 0 && pos.x < (int) visualizer->img->width && pos.y >= 0 && pos.y < (int) visualizer->img->height)
				((uint32_t *) visualizer->img->pixels)[pos.y * visualizer->img->width + pos.x] = color;
			pos.x++;
		}
		pos.x -= cell_size;
		pos.y++;
	}
}

void	draw_board(t_visualizer *visualizer)
{
	for (size_t y = 0; y < BOARD_HEIGHT; y++)
		for (size_t x = 0; x < BOARD_WIDTH; x++)
			draw_cell(visualizer, x, y, visualizer->buffer[y][x]);
}

void	clear_window(mlx_image_t *img, int color)
{
	for (size_t i = 0; i < img->width * img->height; i++)
		((uint32_t *) img->pixels)[i] = color;
}