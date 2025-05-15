#include <visualizer/visualizer.h>

TTF_Font *load_font(const char *path, int32_t fontSize) {
	TTF_Font *font = TTF_OpenFont(path, fontSize);
	if (!font) {
		return (NULL);
	}
	return (font);
}

void unload_font(TTF_Font *font) {
	if (font) {
		TTF_CloseFont(font);
	}
}

void destroy_sdl(t_visualizer *v)
{
	TTF_DestroyRendererTextEngine(v->text_engine);
	unload_font(v->font);
	TTF_Quit();
	SDL_DestroyRenderer(v->renderer);
	SDL_DestroyWindow(v->window);
	SDL_Quit();
}

int init_sdl(t_visualizer *v, char title[], uint32_t width, uint32_t height)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		perror("SDL");
		ft_log(LOG_ERROR, "Failed to init SDL !\n");
		return (1);
	}
	if (!(v->window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE)))
	{
		ft_log(LOG_ERROR, "Failed to init window !\n");
		SDL_Quit();
		return (1);
	}
	
	if (!(v->renderer = SDL_CreateRenderer(v->window, NULL)))
	{
		ft_log(LOG_ERROR, "Failed to init renderer !\n");
		SDL_DestroyWindow(v->window);
		SDL_Quit();
		return (1);
	}
	if (!TTF_Init())
	{
		ft_log(LOG_ERROR, "Failed to init SDL_ttf !\n");
		SDL_DestroyRenderer(v->renderer);
		SDL_DestroyWindow(v->window);
		SDL_Quit();
		return (1);
	}
	if (!(v->font = load_font("/usr/share/fonts/truetype/quicksand/Quicksand-Regular.ttf", 20)))
	{
		ft_log(LOG_ERROR, "Failed to load font !\n");
		TTF_Quit();
		SDL_DestroyRenderer(v->renderer);
		SDL_DestroyWindow(v->window);
		SDL_Quit();
		return (1);
	}
	if (!(v->text_engine = TTF_CreateRendererTextEngine(v->renderer)))
	{
		ft_log(LOG_ERROR, "Failed to create text engine !\n");
		unload_font(v->font);
		TTF_Quit();
		SDL_DestroyRenderer(v->renderer);
		SDL_DestroyWindow(v->window);
		SDL_Quit();
		return (1);
	}
	ft_log(LOG_DEBUG, "SDL successfully initialized !\n");
	return (0);
}
