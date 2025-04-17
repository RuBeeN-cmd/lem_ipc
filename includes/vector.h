typedef struct	s_vec2
{
	int	x;
	int	y;
}				t_vec2;

typedef struct	s_fvec2
{
	float	x;
	float	y;
}				t_fvec2;

int		vec2cmp(t_vec2 v1, t_vec2 v2);
t_vec2	add_vec2(t_vec2 v1, t_vec2 v2);
t_vec2	sub_vec2(t_vec2 v1, t_vec2 v2);
t_vec2	scalar_div_vec2(t_vec2 v, float n);
t_vec2	scalar_mult_vec2(t_vec2 v, float n);
t_vec2	fvec2_to_vec2(t_fvec2 v);

int		fvec2cmp(t_fvec2 v1, t_fvec2 v2);
t_fvec2 add_fvec2(t_fvec2 v1, t_fvec2 v2);
t_fvec2 sub_fvec2(t_fvec2 v1, t_fvec2 v2);
t_fvec2 scalar_div_fvec2(t_fvec2 v, float n);
t_fvec2 scalar_mult_fvec2(t_fvec2 v, float n);
t_fvec2	vec2_to_fvec2(t_vec2 v);