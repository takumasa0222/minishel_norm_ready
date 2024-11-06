# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 21:34:48 by shokosoeno        #+#    #+#              #
#    Updated: 2024/11/06 17:21:22 by ssoeno           ###   ########.fr        #
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
srcs/lexer/lexer.c \
srcs/utils/error.c \
srcs/builtin/builtin_cd.c \
srcs/builtin/builtin_exit.c \
srcs/builtin/builtin_pwd.c \
srcs/builtin/lookup_builtin.c \
srcs/init_env.c \
srcs/map/item_utils.c \
srcs/map/map_get.c \
srcs/map/map_new.c \
srcs/map/map_put.c \
srcs/map/map_set.c \
srcs/map/map_size.c \
srcs/map/map_unset.c

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