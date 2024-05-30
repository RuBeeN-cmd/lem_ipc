_END="\033[0m"
_RED="\033[0;31m"
_GREEN="\033[0;32m"
_YELLOW="\033[0;33m"
_CYAN="\033[0;36m"

NAME = lemipc

SRC = main.c \
		team.c \
		ipc.c \
		ipc_utils.c \
		game.c \
		board.c \
		vec2.c \
		visualizer.c \
		visualizer_ipc.c \
		draw.c \
		buffer.c
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

CC = clang
CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address
INC = -Iincludes -Ilibft/includes -Imlx/include/MLX42

LIB = libft/libft.a mlx/libmlx42.a
LIBFLAGS = $(addprefix -L, $(dir $(LIB))) $(addprefix -l, $(notdir $(subst lib,,$(basename $(LIB)))))

ifeq ($(shell uname), Linux)
	LIBFLAGS += -lm -lglfw -lGL
else ifeq ($(shell uname), Darwin)
	LIBFLAGS += -framework OpenGL -framework AppKit -L/usr/local/Cellar/glfw/3.3.9/lib/ -lglfw -I/usr/local/Cellar/glfw/3.3.9/include/GLFW
endif

SRC_DIR = srcs
OBJ_DIR = objs

all: $(NAME)

$(NAME): $(LIB) $(OBJ_DIR) $(OBJ)
	@echo $(_GREEN)Compiling $(OBJ)...$(_END)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFLAGS) -o $@

%.a:
	@make -C $(dir $@)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo $(_CYAN)Compiling $<...$(_END)
	@$(CC) -o $@ -c $< $(CFLAGS) $(INC)

clean:
	@echo $(_YELLOW)Cleaning $(OBJ)...$(_END)
	@for lib in $(dir $(LIB)); \
		do make -C $$lib fclean; \
	done
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo $(_RED)Cleaning $(NAME)...$(_END)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re