/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/14 11:24:24 by tamatsuu         ###   ########.fr       */
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

t_node	*parse_cmd(t_token **token_list)
{
	t_node	*node;

	//check token_list is null or EOF_NODE
	node = NULL;
	node = parse_cmd_type(token_list);
	if (!node)
		d_throw_error("parse_cmd", "node is empty");//syntax error due to token is not ND_L_PARE/ND_CMD/ND_REDIRECTS
	return (parse_cmd_tail(node, token_list));
}

t_node	*parse_cmd_type(t_token **token_list)
{
	t_node	*node;

	node = NULL;
	if (compare_tk(ND_L_PARE, token_list))
		node = parse_subshell(token_list);
	else
		node = simple_cmd(token_list);
	if (!node)
		d_throw_error("parse_cmd_type", "node is empty");//syntax error due to token is 
	return (node);
}

t_node	*simple_cmd(t_token **token_list)
{
	t_node	*node;
	size_t	cmd_cnt;
	size_t	rd_cnt;

	cmd_cnt = 0;
	rd_cnt = 0;
	node = NULL;
	count_nodes_cmd_rd(token_list, &cmd_cnt, &rd_cnt);
	if (!rd_cnt && cmd_cnt)
	{
		node = create_node(ND_CMD);
		node->cmds = parse_words(token_list);
	}
	else if (!cmd_cnt && rd_cnt && compare_tk(ND_REDIRECTS, token_list))
		node = parse_redirects(token_list);
	else if (rd_cnt && cmd_cnt)
	{
		node = create_node(ND_CMD);
		parse_cmd_rd_node(token_list, node, cmd_cnt, rd_cnt);
	}
	return (node);
}

t_node	*parse_subshell(t_token **token_list)
{
	t_node	*node;

	if (!match_token(ND_L_PARE, token_list))
		throw_unexpected_error("parser_subshell", NULL);
	node = create_node(ND_RND_BRACKET);
	node->left = parse_cmd(token_list);
	if (!match_token(ND_R_PARE, token_list))
		d_throw_error("parser_subshell", "syntax_error");//syntax error
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
		return (NULL);//unexpected error
	ret = xmalloc((word_cnt + 1) * sizeof(char *));
	while (i < word_cnt)
	{
		if (!compare_tk(ND_CMD, token_list))
			d_throw_error("parse_words", "unexpected token type");//unexpected error
		ret[i] = ft_strdup((*token_list)->word);//system error should be replaced xmalloc
		if (!ret[i])
		{
			free_wordlist(ret);
			d_throw_error("parse_words", "strdup_error");
		}
		i++;
		*token_list = (*token_list)->next;
	}
	ret[i] = NULL;
	return (ret);
}
