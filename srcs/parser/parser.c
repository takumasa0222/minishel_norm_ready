/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/19 17:50:51 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"
#include "../../includes/utils.h"

/*
this parser will create AST based on below eBNF.
    start: command
	command: cmd_type command_tail
	command_tail: "|" cmd_type command_tail 
            | ("&&" | "||") cmd_type command_tail
            | redirection
			|
	cmd_type: simple_command | subshell
	subshell: "(" command ")" command_tail*
	simple_command: redirection* wordlist redirection* | redirection+
    wordlist: WORD | wordlist WORD
    redirection: ">" filename
               | "<" filename
               | ">>" filename
               | "<<" filename
               | "<>" filename
               | NUMBER ">" filename
               | NUMBER "<" filename
               | NUMBER ">>" filename
               | NUMBER "<<" filename
               | NUMBER "<>" filename
    filename: WORD
    WORD: /[a-zA-Z0-9_"]+/
    NUMBER: /[0-9]+/
				
*/
t_node	*parse_cmd_handler(t_token **token_list, t_syntax_error *err)
{
	t_node	*ret;
	t_token	*cpy_list;

	cpy_list = *token_list;
	ret = parse_cmd(token_list, err);
	if (!ret && err->is_error)
		throw_syntax_error(err->err_msg, NULL);
	free(err);
	free_token_list(cpy_list);
	return (ret);
}

t_node	*parse_cmd(t_token **tk_list, t_syntax_error *syntx_err)
{
	t_node	*node;

	node = NULL;
	node = parse_cmd_type(tk_list, syntx_err);
	if (!node && syntx_err->is_error)
		return (NULL);
	return (parse_cmd_tail(node, tk_list, syntx_err));
}

t_node	*parse_cmd_type(t_token **tk_list, t_syntax_error *err)
{
	t_node	*node;

	node = NULL;
	if (compare_tk(ND_L_PARE, tk_list))
		node = parse_subshell(tk_list, err);
	else if (compare_tk(ND_CMD, tk_list) || compare_tk(ND_REDIRECTS, tk_list))
		node = simple_cmd(tk_list, err);
	if (!node)
		parser_syntax_err(NULL, err, NULL, ERR_MSG_SYNTAX);
	//if (node->kind == ND_REDIRECTS)
	//if (node && compare_tk(ND_L_PARE, tk_list))
	//	parser_syntax_err(NULL, err, &node, ERR_MSG_L_PARE);
	//if (node && compare_tk(ND_R_PARE, tk_list))
	//	parser_syntax_err(NULL, err, &node, ERR_MSG_R_PARE);
	return (node);
}

t_node	*simple_cmd(t_token **token_list, t_syntax_error *syntx_err)
{
	t_node	*node;
	size_t	cmd_cnt;
	size_t	rd_cnt;

	cmd_cnt = 0;
	rd_cnt = 0;
	node = NULL;
	cnt_cmd_rd(token_list, &cmd_cnt, &rd_cnt, syntx_err);
	if (!rd_cnt && cmd_cnt)
	{
		node = create_node(ND_CMD);
		node->cmds = parse_words(token_list);
	}
	else if (!cmd_cnt && rd_cnt && compare_tk(ND_REDIRECTS, token_list))
		node = parse_redirects(token_list, syntx_err);
	else if (rd_cnt && cmd_cnt)
	{
		node = create_node(ND_CMD);
		parse_cmd_rd_node(token_list, node, cmd_cnt, rd_cnt);
	}
	return (node);
}

char	**parse_words(t_token **token_list)
{
	char	**ret;
	size_t	i;
	size_t	word_cnt;

	i = 0;
	ret = NULL;
	word_cnt = count_nodes(token_list, ND_CMD);
	if (!word_cnt)
		throw_unexpected_error("parse_words", "word_cnt is null");
	ret = xmalloc((word_cnt + 1) * sizeof(char *));
	while (i < word_cnt)
	{
		if (!compare_tk(ND_CMD, token_list) || !(*token_list) \
		|| !(*token_list)->word)
			throw_unexpected_error("parse_words", "unexpected token type");
		ret[i] = x_strdup((*token_list)->word);
		i++;
		*token_list = (*token_list)->next;
	}
	ret[i] = NULL;
	return (ret);
}
