/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/16 04:33:26 by tamatsuu         ###   ########.fr       */
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

/*
今あまり明確になっていないところ
・どんなときならノードを作るのか -> オペレーターもしくはコマンドのときのみ
・親ノードを追加する場合や子ノードを追加するときの切り分け方　-> 実際にほしい木を意識してみるとわかりやすい
・現在の問題点 ((ls)) のような場合にパーサーエラーが発生しない
・match_token を行う場合と単純に判定を行う場合で分ける必要がありそう。基本的には、match は実際のトークン作成時に行うのがベター
・match_token を行う際に実際のトークンも勧めていく形にする
修正ポイント　11/17
・match_token / compare_token の箇所は、t_token_kind に合わせる。一方で、t_token_kind の種類を増やす
*/

t_node	*entry_parser(t_token *token_list)
{
	t_node	*ret;

	ret = NULL;
	return (parse_cmd(&token_list));
}

t_node	*parse_cmd(t_token **token_list)
{
	t_node	*node;

	node = NULL;
	node = parse_cmd_type(token_list);
	if (!node)
		d_throw_error("parse_cmd", "node is empty");
	return (parse_cmd_tail(node, token_list));
}

t_node	*parse_cmd_type(t_token **token_list)
{
	t_node	*node;

	if (compare_token(ND_L_PARE, token_list))
	{
		node = parse_subshell(token_list);
		return (node);
	}	
	else
		return (simple_cmd(token_list));
}

t_node	*simple_cmd(t_token **token_list)
{
	t_node	*node;
	t_node	*tmp;

	node = create_node(ND_CMD);
	if (compare_token(ND_REDIRECTS, token_list))
		node->left = parse_redirects(token_list);
	node->cmds = parse_words(token_list);
	if (!node->cmds && !node->left)
	{
		free(node);
		return (NULL);
	}
	else if (!node->cmds && node->left)
	{
		tmp = node->left;
		free(node);
		return (tmp);
	}
	else if (compare_token(ND_REDIRECTS, token_list))
		node->right = parse_redirects(token_list);
	return (node);
}

char	**parse_words(t_token	**token_list)
{
	char	**ret;
	int		i;
	size_t	word_cnt;

	i = 0;
	ret = NULL;
	word_cnt = count_word_node(token_list);
	if (word_cnt)
		ret = malloc((word_cnt + 1) * sizeof(char *));
	else
		return (ret);
	if (!ret)
		d_throw_error("parse_words", "malloc_error");
	while (compare_token(ND_FD_WORD, token_list))
	{
		ret[i] = (*token_list)->word;
		i++;
		*token_list = (*token_list)->next;
	}
	ret[i] = NULL;
	return (ret);
}

t_node	*parse_redirects(t_token **token_list)
{
	t_node	*node;

	node = NULL;
	if (match_token(ND_REDIRECTS, token_list))
	{
		node = create_node(ND_REDIRECTS);
		node->redirects = parse_redirect_arry(token_list);
	}
	return (node);
}

t_node	*parse_subshell(t_token **token_list)
{
	t_node	*node;

	if (!match_token(ND_L_PARE, token_list))
		d_throw_error("parser_subshell", "syntax_error");
	node = create_node(ND_SUB_SHELL);
	node->left = parse_cmd(token_list);
	if (!match_token(ND_R_PARE, token_list))
		d_throw_error("parser_subshell", "syntax_error");
	return (parse_cmd_tail(node, token_list));
}

t_node	*parse_cmd_tail(t_node *left, t_token **token_list)
{
	t_node	*node;

	node = NULL;
	if (match_token(ND_PIPE, token_list))
	{
		node = create_node(ND_PIPE);
		node->left = left;
		node->right = parse_cmd_type(token_list);
		return (parse_cmd_tail(node, token_list));
	}
	else if (compare_token(ND_AND_OP, token_list) \
	|| compare_token(ND_OR_OP, token_list))
	{
		if (match_token (ND_AND_OP, token_list))
			node = create_node(ND_AND_OP);
		else if (match_token (ND_OR_OP, token_list))
			node = create_node(ND_OR_OP);
		node->left = left;
		node->right = parse_cmd_type(token_list);
		return (parse_cmd_tail(node, token_list));
	}
	else if (compare_token(ND_REDIRECTS, token_list))
		return (parse_redirects(token_list));
	return (left);
}

bool	match_token(t_node_kind expected, t_token **cur_token)
{
	if (!*cur_token)
		return (false);
	if ((*cur_token)->kind == expected)
	{
		*cur_token = (*cur_token)->next;
		return (true);
	}
	return (false);
}

t_node	*create_node(t_node_kind token_kind)
{
	t_node	*new_node;

	new_node = malloc(1 * sizeof(t_node));
	if (!new_node)
		d_throw_error("create_node", "malloc failed");
	new_node->kind = token_kind;
	return (new_node);
}

bool	compare_token(t_node_kind expected, t_token **cur_token)
{
	if (!*cur_token)
		return (false);
	return ((*cur_token)->kind == expected);
}

size_t	count_word_node(t_token	**token_list)
{
	size_t	i;
	t_token	*temp;

	temp = *token_list;
	i = 0;
	while (compare_token(ND_FD_WORD, &temp))
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

char **parse_redirect_arry(t_token	**token_list)
{
	char	**ret;
	int		i;

	i = 0;
	ret = NULL;
	if (i)
		ret = malloc((count_rd_node(token_list) + 1) * sizeof(char *));
	else
		return (ret);
	if (!ret)
		d_throw_error("parse_redirect_arry", "malloc_error");
	while (compare_token(ND_REDIRECTS, token_list))
	{
		ret[i] = (*token_list)->word;
		i++;
		*token_list = (*token_list)->next;
		if (!compare_token(ND_FD_WORD, token_list))
			d_throw_error("parse_redirect_arry", "filename syntax error");
		ret[i] = (*token_list)->word;
		i++;
		*token_list = (*token_list)->next;
	}
	ret[i] = NULL;
	return (ret);
}

size_t	count_rd_node(t_token	**token_list)
{
	size_t	i;
	t_token	*temp;

	temp = *token_list;
	i = 0;
	while (compare_token(ND_REDIRECTS, &temp))
	{
		i++;
		temp = temp->next;
		if (!compare_token(ND_FD_WORD, &temp))
		{
			printf("count_rd_node:unexpexted token\n");
			return (i);
		}
		i++;
		temp = temp->next;
	}
	return (i);
}
