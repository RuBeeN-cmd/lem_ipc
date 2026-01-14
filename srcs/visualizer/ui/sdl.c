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
	DBG("Destroying Renderer...\n");
	SDL_DestroyRenderer(v->renderer);
	DBG("Renderer destroyed.\n");
	SDL_DestroyWindow(v->window);
	SDL_Quit();
}

int init_sdl(t_visualizer *v, char title[], uint32_t width, uint32_t height)
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		ERR("Failed to init SDL\n");
		goto error;
	}
	if (!(v->window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE))) {
		ERR("Failed to init window\n");
		goto quit;
	}
	if (!(v->renderer = SDL_CreateRenderer(v->window, NULL))) {
		ERR("Failed to init renderer\n");
		goto destroy_window;
	}
	if (!TTF_Init()) {
		ERR("Failed to init SDL_ttf\n");
		goto destroy_renderer;
	}
	if (!(v->font = load_font(FONT_PATH, 20))) {
		ERR("Failed to load font\n");
		goto ttf_quit;
	}
	if (!(v->text_engine = TTF_CreateRendererTextEngine(v->renderer))) {
		ERR("Failed to create text engine\n");
		goto unload_font;
	}
	DBG("SDL successfully initialized\n");
	return (0);

	unload_font:		unload_font(v->font);
	ttf_quit:			TTF_Quit();
	destroy_renderer:	SDL_DestroyRenderer(v->renderer);
	destroy_window:		SDL_DestroyWindow(v->window);
	quit:				SDL_Quit();
	error:				return (1);
}
