##
## EPITECH PROJECT, 2025
## MyFTP
## File description:
## Makefile
##

.PHONY: all clean fclean re tests_run unit_tests

SRC_DIR =	src
SRC	=	\
		$(SRC_DIR)/main.c	\
		$(SRC_DIR)/client.c	\
		$(SRC_DIR)/reading.c	\
		$(SRC_DIR)/login.c	\
		$(SRC_DIR)/socket.c	\
		$(SRC_DIR)/command_parser.c	\
		$(SRC_DIR)/command_executor.c	\
		$(SRC_DIR)/utils.c	\
		$(SRC_DIR)/destroyers.c	\
		$(SRC_DIR)/data_socket.c	\
		$(SRC_DIR)/signal.c	\
		$(SRC_DIR)/commands/quit.c	\
		$(SRC_DIR)/commands/exit.c	\
		$(SRC_DIR)/commands/help.c	\
		$(SRC_DIR)/commands/ls.c	\
		$(SRC_DIR)/commands/pwd.c	\
		$(SRC_DIR)/commands/cd.c	\
		$(SRC_DIR)/commands/get.c	\
		$(SRC_DIR)/commands/put.c	\

NAME	=	myftp_client

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
