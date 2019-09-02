CC = clang
CFLAGS = -g
NAME = map_convert
SRCDIR = ./
INCLDIR = ./ -I./libft
LIBSDL = -L./libft -lft -L /Users/trobicho/.brew/lib/ -lSDL2-2.0.0 `sdl2-config --cflags --libs`
SRCS = main.c map.c
OBJ = $(notdir $(SRCS:.c=.o))

all : $(NAME)

$(NAME) : $(OBJ) $(LIB)
	@$(CC) $(CFLAGS) -lm -I $(INCLDIR) $^ -o $@ $(LIBSDL)
	@echo "\n\033[36mCreation :\033[0m \033[35;4m$(NAME)\033[0m\n"

%.o: %.c
	@$(CC) $(CFLAGS) -c $^ -I $(INCLDIR) -I /Users/trobicho/.brew/include/SDL2
	@echo "\033[36mCompilation :\033[0m \033[32m$*\033[0m"

clean :
	@rm -rf $(OBJ)
	@echo "\n\033[36mDeletion :\033[0m \033[32mObjects\033[0m\n"

fclean : clean
	@rm -rf $(NAME)
	@echo "\033[36mDeletion :\033[0m \033[35;4m$(NAME)\033[0m\n"

re : fclean all

.PHONY : all clean fclean re
