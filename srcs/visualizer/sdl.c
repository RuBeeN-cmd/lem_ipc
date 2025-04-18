#include "lem_ipc.h"

int init_sdl(t_visualizer *v, char title[], uint32_t width, uint32_t height)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		perror("SDL");
		ft_log(LOG_ERROR, "Failed to init SDL !\n");
		return (1);
	}
	ft_log(LOG_DEBUG, "SDL successfully initialized !\n");
	v->window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
	if (!v->window)
	{
		ft_log(LOG_ERROR, "Failed to init window !\n");
		SDL_Quit();
		return (1);
	}
	ft_log(LOG_DEBUG, "Window successfully initialized !\n");
	v->renderer = SDL_CreateRenderer(v->window, NULL);
	if (!v->renderer)
	{
		ft_log(LOG_ERROR, "Failed to init renderer !\n");
		SDL_DestroyWindow(v->window);
		SDL_Quit();
		return (1);
	}
	ft_log(LOG_DEBUG, "Renderer successfully initialized !\n");
	return (0);
}
