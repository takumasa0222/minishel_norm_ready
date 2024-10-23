/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 00:03:57 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/10/24 03:46:03 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include <stdio.h>
#include <stdbool.h>
#include "../../libft/libft.h"

/*
This function divides char pointer into token pointer list.
Token pointer list is made of following
words , meta charactors, operators and reserved words.
word : A sequence of characters considered as a single unit by the shell.
Also known as a token.
metacharacter : A character that, when unquoted, separates words.
One of the following:
 & ; ( ) < > space tab
Operator : A token that performs a control function.
It is one of the following symbols:
|| & && ; ;; () | |& <newline>
*/
t_token	*lexer(char *line)
{
	t_token	head;
	t_token	*token;

	head.next = NULL;
	token = &head;
	while (line)
	{
		if (skip_blank(&line, line))
			continue ;
		else if (is_operator(line))
			token->next = fetch_fst_ope_token(line);
		else if (is_word(line))
			token->next = fetch_fst_word_token(line);
		else
			throw_error("unexpected token");
		token = token->next;
		line = line + ft_strlen(token);
	}
	token->next = new_token(NULL, TK_EOF);
	return (head.next);
}

bool	skip_blank(char **rest, char *input)
{
	size_t	i;

	if (!rest)
		throw_error("skip_blank");
	i = 0;
	while (input[i] && is_blank(input[i]))
		i++;
	*rest = i + *rest;
	return (i);
}

bool	is_operator(char *input)
{
	size_t		i;
	size_t		ope_len;
	static char	**ope = \
	{"||", "|", "&&", "&", ";;", ";", "(", ")", "|&", "\n", NULL};

	if (!input)
		throw_error("is_operator");
	i = 0;
	ope_len = get_char_arry_size(ope);
	while (i < ope_len)
	{
		if (!ft_strncmp(ope[i], input, ft_strlen(ope[i])))
			return (true);
		i++;
	}
	return (false);
}

size_t	get_char_arry_size(char **str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i])
		i++;
	return (i);
}

bool	is_metachar(int c)
{
	return (c && ft_strchr("|&;()<> \t\n", c));
}

bool	is_word(char *input)
{
	return (input && !is_metachar(*input));
}

bool	is_blank(int c)
{
	return (ft_strchr(" \t", c));
}

t_token	*fetch_fst_ope_token(char *input)
{
	
}

t_token	*fetc_fst_word_token(char *input)
{
	
}
