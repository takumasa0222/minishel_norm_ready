/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 00:03:57 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/13 22:34:18 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../libft/libft.h"
#include "../../includes/utils.h"
#include "../../includes/minishell.h"

t_token	*lexer_handler(char *line, t_syntax_err **syntax_err, t_context *ctx)
{
	t_token	*ret;

	ret = NULL;
	ret = lexer(line, *syntax_err);
	if ((*syntax_err)->is_err)
	{
		printf("%s\n", (*syntax_err)->err_msg);
		printf("syntax error: unexpected end of file\n");
		ctx->last_status = EXIT_FAILURE;
		free_token_list(ret);
		free(*syntax_err);
		*syntax_err = NULL;
		return (NULL);
	}
	return (ret);
}

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
t_token	*lexer(char *line, t_syntax_err *syntax_err)
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
			token->next = fetch_fst_word_token(line, syntax_err);
		else
			throw_unexpected_error("lexer", NULL);
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
		throw_unexpected_error("skip_blank", "rest is NULL");
	i = 0;
	while (input[i] && is_blank(input[i]))
		i++;
	*rest = i + input;
	return (i);
}

t_token	*fetch_fst_word_token(char *input, t_syntax_err *syntax_err)
{
	size_t		i;

	if (!input)
		throw_unexpected_error("fetch_fst_word_token", "input is NULL");
	i = 0;
	while (input[i])
	{
		if (is_s_quote(input[i]) || is_d_quote(input[i]))
			i = move_to_next_quotation(input, i, syntax_err);
		if (is_metachar(input[i]) || syntax_err->is_err)
			break ;
		i++;
	}
	if (i)
		return (create_token(x_substr(input, 0, i), ND_CMD));
	throw_unexpected_error("fetch_fst_word_token", NULL);
	return (NULL);
}

int	move_to_next_quotation(char *input, int i, t_syntax_err *syntax_err)
{
	if (is_s_quote(input[i]))
	{
		i++;
		while (input[i] && !is_s_quote(input[i]))
			i++;
		if (!input[i] && !syntax_err->is_err)
		{
			syntax_err->is_err = true;
			syntax_err->err_msg = ERR_MSG_SYNTAX_S_QUOTE;
		}
	}
	if (is_d_quote(input[i]))
	{
		i++;
		while (input[i] && !is_d_quote(input[i]))
			i++;
		if (!input[i] && !syntax_err->is_err)
		{
			syntax_err->is_err = true;
			syntax_err->err_msg = ERR_MSG_SYNTAX_D_QUOTE;
		}
	}
	return (i);
}
