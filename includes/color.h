#include <stdint.h>

typedef struct	s_color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
}				t_color;

t_color color_from_u32(uint32_t input);