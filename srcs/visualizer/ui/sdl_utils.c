#include <visualizer/sdl_utils.h>

t_vec2	get_text_size(TTF_Text *text)
{
	t_vec2 size;
	if (!TTF_GetTextSize(text, &size.x, &size.y))
		return (NULL_SIZE);
	return (size);
}

t_vec2	get_renderer_size(SDL_Renderer *renderer)
{
	t_vec2 size;
	if (!SDL_GetCurrentRenderOutputSize(renderer, &size.x, &size.y))
		return (NULL_SIZE);
	return (size);
}