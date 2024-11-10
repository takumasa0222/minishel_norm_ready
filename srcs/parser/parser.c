/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:51:38 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/10 20:37:47 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"
#include "../../includes/parser.h"

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

t_node	*parse_command() {
    t_node	*node;

	node = parse_cmd_type();
	if (!node)
		d_throw_error();
    return (parse_command_tail(node));
}


t_node	*cmd_type(t_node **ast_tr)
{
	t_node	*ret;

	if (match_subshell())
	{
		subshell();
	}
	else
	{
		simple_command();
		simple_command1();
	}
}

t_node	*simple_command()
{
	t_node	*node;

	while (redirection())
	{
	}
	if (!node)
		word_list();
	while (redirection())
	{
	}
}

t_node	*simple_command1()
{
	redirection();
}

t_node	*redirection()
{
	
}

t_node	*subshell(t_node **ast_tr)
{
	
	if (token)
}

t_node	*parse_command_tail(t_node *left)
{
	t_node	*node;

	if (match_token(ND_PIPE)) 
	{
		node = create_node(ND_PIPE);
		node->left = left;
        node->right = parse_cmd_type();
		return (parse_command_tail(node));
	}
	else if (match_token(ND_AND_OP) || match_token(ND_OR_OP))
	{
		if (match_token (ND_AND_OP))
			node = create_node(ND_AND_OP);
		else
			node = create_node(ND_OR_OP);
		node->left = left;
		node->right = parse_cmd_type();
		return (parse_command_tail(node));
	}
	else if (is_redirection(current_token))
		return (parse_redirection(left));
    return (left);
}

bool	match_token(t_node_kind expected, t_token **cur_token)
{
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

match_subshell()
{
	
}