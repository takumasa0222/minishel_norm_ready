# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 21:34:48 by shokosoeno        #+#    #+#              #
#    Updated: 2025/01/10 23:27:16 by tamatsuu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Configure settings according to the OS
UNAME_S := $(shell uname -s)

NAME = minishell
CC = $(if $(findstring Darwin, $(UNAME_S)), cc, clang)
INCLUDES = -I includes -I $(LIBFT_DIR)
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

# add libft
LIBFT_DIR = ./libft
LIBFT = ${LIBFT_DIR}/libft.a

SRCS = srcs/main.c \
srcs/lexer/lexer_helper.c \
srcs/lexer/lexer_token_checker.c \
srcs/lexer/lexer_token_quote_checker.c \
srcs/lexer/lexer_operator_checker.c \
srcs/lexer/lexer.c \
srcs/parser/parser.c \
srcs/parser/parser_cmd_tail.c \
srcs/parser/parser_redirect_word.c \
srcs/parser/parser_token_checker.c \
srcs/parser/parser_node_operator.c \
srcs/utils/error.c \
srcs/utils/free_functions.c \
srcs/utils/ft_strcmp.c \
srcs/utils/init_env.c \
srcs/utils/xmalloc.c \
srcs/expand/expand_handler.c \
srcs/expand/expand_helper.c \
srcs/expand/expand_variable_helper.c \
srcs/expand/expand_variable.c \
srcs/expand/remove_quotes.c \
srcs/builtin/builtin_cd.c \
srcs/builtin/builtin_cd_utils.c \
srcs/builtin/builtin_exit.c \
srcs/builtin/builtin_pwd.c \
srcs/builtin/builtin_export.c \
srcs/builtin/builtin_export_utils.c \
srcs/builtin/builtin_unset.c \
srcs/builtin/builtin_echo.c \
srcs/builtin/builtin_env.c \
srcs/builtin/lookup_builtin.c \
srcs/map/item_utils.c \
srcs/map/map_get.c \
srcs/map/map_new.c \
srcs/map/map_put.c \
srcs/map/map_set.c \
srcs/map/map_size.c \
srcs/map/map_unset.c \
srcs/signal/signal.c \
srcs/signal/signal_child.c \
srcs/execution/execute_fd_controller.c \
srcs/execution/execute_logical_operator.c \
srcs/execution/execute_utils.c \
srcs/execution/execute.c \
srcs/execution/execute_prep.c \
srcs/heredoc/heredoc.c \
srcs/redirect/redirect.c \
get_next_line/get_next_line.c \
get_next_line/get_next_line_utils.c 


OBJS = $(SRCS:.c=.o)

# General rules

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

test: all
	./test.sh

.PHONY: all clean fclean re test