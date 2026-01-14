#include <lem_ipc.h>

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
	team = team % 36;
	int mod_team = (team - 1) % 12;
	int hue = mod_team * 360 / 3.;
	if (mod_team >= 9)
		hue += 360 / 6. + 360 / 12.;
	else if (mod_team >= 6)
		hue += 360 / 12.;
	else if (mod_team >= 3)
		hue += 360 / 6.;
	hue = hue % 360;

	float lightness = 0.5;
	if (team > 24)
		lightness = 0.75;
	else if (team > 12)
		lightness = 0.25;
	return (hsl_to_rgb(hue, 1, lightness));
}

static uint32_t	get_team_from_str(const char *nptr)
{
	unsigned int	nb;
	char			*str;

	nb = 0;
	str = (char *) nptr;
	while (*str >= '0' && *str <= '9')
	{
		if (nb > (nb + *str - '0') * 10)
			return (0);
		nb *= 10;
		nb += *str++ - '0';
	}
	if (*str)
		return (0);
	return (nb);
}

uint32_t	get_team(char *team_str)
{
	if (!ft_strncmp(team_str, "visualizer", 11))
		return (VISUALIZER);
	uint32_t team = get_team_from_str(team_str);
	if (team < 1 || team > MAX_TEAMS)
		return (0);
	return (team);
}

uint32_t	get_team_on_board(t_vec2 target, uint32_t *board[], t_vec2 board_size) {
	if (target.x < 0 || target.x >= board_size.x
		|| target.y < 0 || target.y >= board_size.y)
		return (VISUALIZER); // Error case
	else
		return (board[target.y][target.x]);
}