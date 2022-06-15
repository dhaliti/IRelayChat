# ==---------------------== #
#                           #
#        PARAMETERS         #
#                           #
# ==---------------------== #

NAME = ircserv
ARGS = 8889 pp

FLAGS = -Wall -Werror -Wextra -std=c++98 -g3
DANGER = -fsanitize=address

SRC = BOT.cpp newClient.cpp utils.cpp commands.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

# ==---------------------== #
#                           #
#           RULES           #
#                           #
# ==---------------------== #

all: $(NAME)

.cpp.o:
	@c++ $(FLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	@c++ $(DANGER) -o $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

test: all
	@./$(NAME) $(ARGS)

update:
	@git pull

re: fclean all

.PHONY: all clean fclean re
