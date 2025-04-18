#include "vector.h"

int	fvec2cmp(t_fvec2 v1, t_fvec2 v2) {
	return (v1.x != v2.x || v1.y != v2.y);
}

t_fvec2 add_fvec2(t_fvec2 v1, t_fvec2 v2)
{
	return ((t_fvec2) {
		v1.x + v2.x,
		v1.y + v2.y
	});
}

t_fvec2 sub_fvec2(t_fvec2 v1, t_fvec2 v2)
{
	return ((t_fvec2) {
		v1.x - v2.x,
		v1.y - v2.y
	});
}

t_fvec2 scalar_div_fvec2(t_fvec2 v, float n)
{
	return ((t_fvec2) {
		v.x / n,
		v.y / n
	});
}

t_fvec2 scalar_mult_fvec2(t_fvec2 v, float n)
{
	return ((t_fvec2) {
		v.x * n,
		v.y * n
	});
}

t_fvec2	vec2_to_fvec2(t_vec2 v)
{
	return ((t_fvec2) {
		v.x,
		v.y
	});
}