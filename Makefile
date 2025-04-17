_END="\033[0m"
_RED="\033[0;31m"
_GREEN="\033[0;32m"
_YELLOW="\033[0;33m"
_CYAN="\033[0;36m"

NAME = lemipc

SRC = main.c \
		team.c \
		init_ipc.c \
		init_game.c \
		ipc.c \
		ipc_utils.c \
		game.c \
		board.c \
		vector/vec2.c \
		vector/fvec2.c \
		visualizer.c \
		visualizer_ipc.c \
		draw.c \
		buffer.c \
		color.c \
		log.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

CC = clang
CFLAGS = -Wall -Werror -Wextra -g3 #-fsanitize=address
INC = -Iincludes -Ilibft/includes -ISDL3-3.2.10/include

LIB = libft/libft.a SDL3-3.2.10/libSDL3.so
LIBFLAGS = $(addprefix -L, $(dir $(LIB))) $(addprefix -l, $(notdir $(subst lib,,$(basename $(LIB)))))
LIBFLAGS += -Wl,-rpath,SDL3-3.2.10
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