##
## EPITECH PROJECT, 2025
## MyFTP
## File description:
## Makefile
##

.PHONY: all clean fclean re tests_run unit_tests

SRC	=	\
		src/main.c	\

NAME	=	myftp

OBJ	=	$(SRC:.c=.o)

CFLAGS += -I./include/ -Werror -Wall -Wextra -g

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(OBJ)	\

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all

NAME_TEST	=	unit_tests

SRC_TESTS =	\
			tests/test.c	\

FLAGS_TEST = \
			-lcriterion --coverage

unit_tests: re
	gcc -o $(NAME_TEST) $(SRC_TESTS) $(CFLAGS) $(FLAGS_TEST)

tests_run: unit_tests
	./$(NAME_TEST)

coverage: unit_tests
	./$(NAME_TEST)
	gcovr --exclude tests/
	gcovr --exclude tests/ --txt-metric branch
	rm $(NAME_TEST)
