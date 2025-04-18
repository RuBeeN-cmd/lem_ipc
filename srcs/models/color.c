#include "color.h"

t_color color_from_u32(uint32_t input)
{
    return *((t_color *) &input);
}
