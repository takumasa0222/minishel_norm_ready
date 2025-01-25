/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 00:03:57 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/25 13:24:14 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../libft/libft.h"
#include "../../includes/utils.h"

/*
This function divides char pointer into token pointer list.
Token pointer list is made of following :
word : A sequence of characters considered as a single unit by the shell.
Also known as a token.
metacharacter : A character that, when unquoted, separates words.
One of the following: & ; ( ) < > space tab
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
	while (*line)
	{
		if (skip_blank(&line, line))
			continue ;
		else if (is_operator(line))
			token->next = fetch_fst_ope_token(line);
		else if (is_word(line))
		{
			token->next = fetch_fst_word_token(line);
			if (!token->next) // free partial token list
				return (free_token_list(head.next), NULL);
		}
		else
			d_throw_error("lexer", "unexpected token");
		line = line + ft_strlen(token->next->word);
		token = token->next;
	}
	token->next = create_token(NULL, ND_EOF);
	return (head.next);
}

bool	skip_blank(char **rest, char *input)
{
	size_t	i;

	if (!rest)
		d_throw_error("skip_blank", "rest is NULL");
	i = 0;
	while (input[i] && is_blank(input[i]))
		i++;
	*rest = i + input;
	return (i);
}

t_token	*fetch_fst_word_token(char *input)
{
	size_t		i;

	if (!input)
		d_throw_error("fetch_fst_word_token", "input is NULL");
	i = 0;
	while (input[i])
	{
		if (is_s_quote(input[i]) || is_d_quote(input[i]))
			i = move_to_next_quotation(input, i);
		if (is_metachar(input[i]))
			break ;
		i++;
	}
	if (i)
		return (create_token(ft_substr(input, 0, i), ND_CMD));
	// d_throw_error("fetch_fst_word_token", "logically unexpected error");
	return (NULL);
}

int	move_to_next_quotation(char *input, int i)
{
	if (is_s_quote(input[i]))
	{
		i++;
		while (input[i] && !is_s_quote(input[i]))
			i++;
		if (!input[i])
			d_throw_error("move_to_next_quotation", "squote not closed");
	}
	if (is_d_quote(input[i]))
	{
		i++;
		while (input[i] && !is_d_quote(input[i]))
			i++;
		if (!input[i])
			d_throw_error("move_to_next_quotation", "dquote not closed");
	}
	return (i);
}
