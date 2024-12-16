# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 21:34:48 by shokosoeno        #+#    #+#              #
#    Updated: 2024/12/16 23:15:25 by tamatsuu         ###   ########.fr        #
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
	# LIBS := -lreadline
endif

SRCS = ./expand_tester.c \
../../srcs/map/map_new.c \
../../srcs/map/map_put.c \
../../srcs/map/map_get.c \
../../srcs/map/map_set.c \
../../srcs/map/item_utils.c \
../../srcs/utils/error.c \
../../srcs/expand/expand.c \
../../srcs/lexer/lexer_token_quote_checker.c


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

test: all
	./test.sh

.PHONY: all clean fclean re test