_END="\033[0m"
_RED="\033[0;31m"
_GREEN="\033[0;32m"
_YELLOW="\033[0;33m"
_CYAN="\033[0;36m"

NAME = lemipc

SRC = main.c \
		parsing/args.c \
		parsing/team.c \
		player/player.c \
		player/game.c \
		player/init_game.c \
		player/ipc.c \
		player/messages.c \
		ipc/ipc.c \
		ipc/sem.c \
		ipc/shm.c \
		ipc/msg.c \
		visualizer/visualizer.c \
		visualizer/draw.c \
		visualizer/shared_memory.c \
		visualizer/kills.c \
		visualizer/supervision.c \
		visualizer/game.c \
		visualizer/ui/draw.c \
		visualizer/ui/events.c \
		visualizer/ui/sdl.c \
		visualizer/ui/panel.c \
		visualizer/ui/text_line.c \
		models/vector/vec2.c \
		models/vector/fvec2.c \
		models/color.c \
		utils/log.c \
		utils/ansi_color.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

CC = clang
CFLAGS = -Wall -Werror -Wextra -gdwarf-4 #-fsanitize=address
INC = -Iincludes -Ilibft/includes -ISDL3-3.2.10/include -ISDL3_ttf-3.2.2/include

LIB = libft/libft.a SDL3-3.2.10/libSDL3.so SDL3_ttf-3.2.2/libSDL3_ttf.so
LIBFLAGS = $(addprefix -L, $(dir $(LIB))) $(addprefix -l, $(notdir $(subst lib,,$(basename $(LIB)))))
LIBFLAGS += -Wl,-rpath,SDL3-3.2.10 -Wl,-rpath,SDL3_ttf-3.2.2
LIBFLAGS += -lm

SRC_DIR = srcs
OBJ_DIR = objs

all: $(NAME)

$(NAME): $(LIB) $(OBJ_DIR) $(OBJ)
	@echo $(_GREEN)Compiling $(OBJ)...$(_END)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFLAGS) -o $@

%.a %.so:
	@make -C $(dir $@)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo $(_CYAN)Compiling $<...$(_END)
	@$(CC) -o $@ -c $< $(CFLAGS) $(INC)

clean:
	@echo $(_YELLOW)Cleaning $(OBJ)...$(_END)
# @for lib in $(dir $(LIB)); \
# 	do make -C $$lib fclean
# done
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo $(_RED)Cleaning $(NAME)...$(_END)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re