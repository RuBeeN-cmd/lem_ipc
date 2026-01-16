#include <visualizer/sdl.h>

t_vec2	get_text_size(TTF_Text *text)
{
	t_vec2 size;
	if (!TTF_GetTextSize(text, &size.x, &size.y))
		return (NULL_SIZE);
	return (size);
}