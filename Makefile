# ------------ COLORS --------------

_END="\033[0m"
_RED="\033[0;31m"
_GREEN="\033[0;32m"
_YELLOW="\033[0;33m"
_CYAN="\033[0;36m"

# ----------------------------------

NAME = lemipc

SRC_DIR = srcs
OBJ_DIR = objs

SRC = main.c \
		parsing/args.c \
		parsing/team.c \
		player/player.c \
		player/movement.c \
		player/init_game.c \
		player/ipc.c \
		player/messages.c \
		player/workflow.c \
		player/game_utils.c \
		ipc/ipc.c \
		ipc/sem.c \
		ipc/shm.c \
		ipc/msg.c \
		visualizer/visualizer.c \
		visualizer/draw.c \
		visualizer/shared_memory.c \
		visualizer/kills.c \
		visualizer/supervision.c \
		visualizer/game_utils.c \
		visualizer/ui/draw.c \
		visualizer/ui/events.c \
		visualizer/ui/sdl.c \
		visualizer/ui/sdl_utils.c \
		visualizer/ui/panel.c \
		visualizer/ui/text_line.c \
		models/vector/vec2.c \
		models/vector/fvec2.c \
		models/color.c \
		utils/log.c \
		utils/ansi_color.c


CC = clang
CFLAGS = -Wall -Werror -Wextra -gdwarf-4 # -fsanitize=address
INC = -Iincludes

# ------------- LIBS ----------------

LIBFT_DIR = libft
LIBFT_INCLUDE = $(LIBFT_DIR)/includes
LIBFT = $(LIBFT_DIR)/libft.a

SDL_DIR     = SDL3-3.2.30
SDL_BUILD   = $(SDL_DIR)/build
SDL_INSTALL = $(abspath $(SDL_DIR)/install)
SDL_CONFIG  = $(SDL_BUILD)/CMakeCache.txt
SDL_BUILD_LIB = $(SDL_BUILD)/libSDL3.so
SDL_INCLUDE = $(SDL_INSTALL)/include
SDL     	= $(SDL_INSTALL)/lib/libSDL3.so

SDL_TTF_DIR     = SDL3_ttf-3.2.2
SDL_TTF_BUILD   = $(SDL_TTF_DIR)/build
SDL_TTF_INSTALL = $(SDL_TTF_DIR)/install
SDL_TTF_CONFIG  = $(SDL_TTF_BUILD)/CMakeCache.txt
SDL_TTF_BUILD_LIB = $(SDL_TTF_BUILD)/libSDL3_ttf.so
SDL_TTF_INCLUDE = $(SDL_TTF_INSTALL)/include
SDL_TTF       	= $(SDL_TTF_INSTALL)/lib/libSDL3_ttf.so

LIB = $(LIBFT) $(SDL) $(SDL_TTF)
LIBFLAGS = -L$(dir $(SDL)) -L$(dir $(SDL_TTF)) -L$(dir $(LIBFT))
LIBFLAGS += -Wl,-rpath,$(dir $(SDL)) -Wl,-rpath,$(dir $(SDL_TTF))
LIBFLAGS += -lSDL3 -lSDL3_ttf -lft
LIBFLAGS += -lm
INC += -I$(LIBFT_INCLUDE) -I$(SDL_INCLUDE) -I$(SDL_TTF_INCLUDE)
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# ----------------------------------

all: $(NAME)

$(NAME): $(LIB) $(OBJ_DIR) $(OBJ)
	@echo $(_GREEN)Compiling $(OBJ)...$(_END)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFLAGS) -o $@

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo $(_CYAN)Compiling $<...$(_END)
	@$(CC) -o $@ -c $< $(CFLAGS) $(INC)

# ------- LIBFT -------

%.a:
	@make -C $(dir $@)
	
# ---------- SDL ----------

$(SDL_CONFIG):
	@cmake -S $(SDL_DIR) -B $(SDL_BUILD)

$(SDL_BUILD_LIB): $(SDL_CONFIG)
	@cmake --build $(SDL_BUILD)

SDL_STAMP = $(SDL_INSTALL)/.installed

$(SDL_STAMP): $(SDL_BUILD_LIB) $(SDL_CONFIG)
	@cmake --install $(SDL_BUILD) --prefix $(SDL_INSTALL)
	@touch $(SDL_STAMP)

$(SDL): $(SDL_STAMP)

# ---------- SDL_ttf ----------

$(SDL_TTF_CONFIG): $(SDL_STAMP)
	@cmake -S $(SDL_TTF_DIR) -B $(SDL_TTF_BUILD) \
		-DCMAKE_PREFIX_PATH=$(SDL_INSTALL)

$(SDL_TTF_BUILD_LIB): $(SDL_TTF_CONFIG)
	@cmake --build $(SDL_TTF_BUILD)

SDL_TTF_STAMP = $(SDL_TTF_INSTALL)/.installed

$(SDL_TTF_STAMP): $(SDL_TTF_BUILD_LIB) $(SDL_TTF_CONFIG)
	@cmake --install $(SDL_TTF_BUILD) --prefix $(SDL_TTF_INSTALL)
	@touch $(SDL_TTF_STAMP)

$(SDL_TTF): $(SDL_TTF_STAMP)

# ---------- Public targets ----------

libft: $(LIBFT)
libft_clean:
	@make -C $(LIBFT_DIR) fclean

sdl: $(SDL)
sdl_clean:
	@echo $(_RED)Cleaning $(SDL_BUILD) $(SDL_INSTALL)...$(_END)
	@rm -rf $(SDL_BUILD) $(SDL_INSTALL) $(SDL_STAMP)

sdl_ttf: $(SDL_TTF)
sdl_ttf_clean:
	@echo $(_RED)Cleaning $(SDL_TTF_BUILD) $(SDL_TTF_INSTALL)...$(_END)
	@rm -rf $(SDL_TTF_BUILD) $(SDL_TTF_INSTALL) $(SDL_TTF_STAMP)

clean:
	@echo $(_YELLOW)Cleaning $(OBJ)...$(_END)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo $(_RED)Cleaning $(NAME)...$(_END)
	@rm -f $(NAME)

ffclean: fclean libft_clean sdl_clean sdl_ttf_clean

re: fclean all

.PHONY: all clean fclean re ffclean libft sdl sdl_ttf libft_clean sdl_clean sdl_ttf_clean