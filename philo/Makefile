FUNCTS = main.c positive_atoi.c ft_strlen.c time_utils.c

CC = cc -pthread

CFLAGS= -Wall -Werror -Wextra  -g -fsanitize=thread

NAME=philo

O_FILES=$(FUNCTS:.c=.o)

all: $(NAME)

$(NAME):  $(O_FILES)
		$(CC) $(CFLAGS) $(O_FILES) -o philo
clean:
	rm -rf $(O_FILES)

fclean:
	rm -rf $(O_FILES) $(NAME)

re:	fclean all

.PHONY : re all fclean clean
