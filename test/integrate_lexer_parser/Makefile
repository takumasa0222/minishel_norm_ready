# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 21:34:48 by shokosoeno        #+#    #+#              #
#    Updated: 2024/12/22 20:38:05 by ssoeno           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Configure settings according to the OS
UNAME_S := $(shell uname -s)

NAME = minishell
CC = $(if $(findstring Darwin, $(UNAME_S)), cc, clang)
INCLUDES = -I ../../includes
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES)

ifeq ($(UNAME_S),Darwin)
	# for macOS
	RLDIR := $(shell brew --prefix readline)
	LIBS := -L$(RLDIR)/lib -lreadline
	INCLUDES += -I$(RLDIR)/include
else
	# for Linux
	LIBS := -lreadline
endif


SRCS = \
./lexer_parser_test.c \
../../srcs/utils/error.c \
../../srcs/utils/free_functions.c \
../../srcs/utils/xmalloc.c \
../../srcs/parser/parser.c \
../../srcs/parser/parser_cmd_tail.c \
../../srcs/parser/parser_redirect_word.c \
../../srcs/parser/parser_token_checker.c \
../../srcs/parser/parser_node_operator.c \
../../srcs/lexer/lexer_helper.c \
../../srcs/lexer/lexer_token_checker.c \
../../srcs/lexer/lexer_token_quote_checker.c \
../../srcs/lexer/lexer_operator_checker.c \
../../srcs/lexer/lexer.c
# ./sample.c \

# SRCS = \
# ./lexer_parser_test.c \
# ../../srcs/utils/error.c \
# ../../srcs/parser/parser.c \
# ../../srcs/parser/parser_redirect_word.c \
# ../../srcs/parser/parser_token_checker.c \
# ../../srcs/parser/parser_node_operator.c \
# ../../srcs/lexer/lexer_helper.c \
# ../../srcs/lexer/lexer_token_checker.c \
# ../../srcs/lexer/lexer_token_quote_checker.c \
# ../../srcs/lexer/lexer_operator_checker.c \
# ../../srcs/lexer/lexer.c
#./ft_get_next_line/get_next_line_bonus.c \
#./ft_get_next_line/get_next_line_utils_bonus.c \
#../../srcs/lexer/lexer_helper.c \
#../../srcs/lexer/lexer_token_checker.c \
#../../srcs/lexer/lexer_token_quote_checker.c \
#../../srcs/lexer/lexer.c \

OBJS = $(SRCS:.c=.o)

# General rules

all: $(NAME)

$(NAME): $(OBJS)
	@make -C ../../libft
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -L../../libft -lft -o $(NAME) 

srcs/%.o: srcs/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test