#include "lem_ipc.h"

static float hue2rgb(float p, float q, float t) {
	if (t < 0.0f) t += 1.0f;
	if (t > 1.0f) t -= 1.0f;
	if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
	if (t < 1.0f / 2.0f) return q;
	if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
	return p;
}

t_color hsl_to_rgb(uint16_t h_deg, float s, float l) {
	float h = (h_deg % 360) / 360.0f;
	float r, g, b;

	if (s == 0.0f) {
		r = g = b = l;
	} else {
		float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
		float p = 2.0f * l - q;
		r = hue2rgb(p, q, h + 1.0f / 3.0f);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1.0f / 3.0f);
	}
	return ((t_color) {
		255 * r,
		255 * g,
		255 * b,
		255
	});
}

t_color	get_team_color(int team)
{
	if (team == 0)
		return (color_from_u32(0xFFFFFFFF));
	team = team - 1;
	int hue = team * 360 / 3.;
	if (team >= 3)
		hue += 360 / 6.;
	hue = hue % 360;

	float lightness = 0.5;
	if (team >= 9)
		lightness = 0.1;
	else if (team >= 6)
		lightness = 0.8;
	return (hsl_to_rgb(hue, 1, lightness));
}

void	draw_cell(t_visualizer *v, int x, int y, int team, uint32_t renderer_width, uint32_t renderer_height)
{
	t_color		team_color = get_team_color(team);
	size_t		cell_size = v->cell_size;
	t_vec2		pos = {
		(x - (float) (BOARD_WIDTH / 2) - v->offset.x) * cell_size + renderer_width / 2 + BORDER_WIDTH / 2,
		(y - (float) (BOARD_HEIGHT / 2) - v->offset.y) * cell_size + renderer_height / 2 + BORDER_WIDTH / 2
	};
	SDL_FRect	cell = {pos.x, pos.y, cell_size - BORDER_WIDTH, cell_size - BORDER_WIDTH};
	SDL_SetRenderDrawColor(v->renderer, team_color.r, team_color.g, team_color.b, team_color.a);
	SDL_RenderFillRect(v->renderer, &cell);
}

void	draw_board(t_visualizer *v)
{
	uint32_t renderer_width;
	uint32_t renderer_height;
	SDL_GetCurrentRenderOutputSize(v->renderer, (int *) &renderer_width, (int *) &renderer_height);
	for (size_t y = 0; y < BOARD_HEIGHT; y++)
		for (size_t x = 0; x < BOARD_WIDTH; x++)
			draw_cell(v, x, y, v->buffer[y][x], renderer_width, renderer_height);
}

void	clear_window(SDL_Renderer *renderer, t_color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer);
}