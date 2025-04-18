#include "vector.h"

int	vec2cmp(t_vec2 v1, t_vec2 v2) {
	return (v1.x != v2.x || v1.y != v2.y);
}

t_vec2	add_vec2(t_vec2 v1, t_vec2 v2)
{
	return ((t_vec2) {
		v1.x + v2.x,
		v1.y + v2.y
	});
}

t_vec2	sub_vec2(t_vec2 v1, t_vec2 v2)
{
	return ((t_vec2) {
		v1.x - v2.x,
		v1.y - v2.y
	});
}

t_vec2	scalar_div_vec2(t_vec2 v, float n)
{
	return ((t_vec2) {
		v.x / n,
		v.y / n
	});
}

t_vec2	scalar_mult_vec2(t_vec2 v, float n)
{
	return ((t_vec2) {
		v.x * n,
		v.y * n
	});
}

t_vec2	fvec2_to_vec2(t_fvec2 v)
{
	return ((t_vec2) {
		v.x,
		v.y
	});
}