/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:57:36 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/13 16:44:42 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"

void	expand_handler(t_node *node, t_context *ctx)
{
	if (!node || !node->cmds)
		d_throw_error("expand_handler", "node or cmds is null");//unexpected error
	expand_variable_handler(node, ctx);
	//word_split_handler(node);
	//expand_asterisk_handler(node);
	remove_quote_handler(node);
}

void	expand_variable_handler(t_node *node, t_context *ctx)
{
	int		i;
	char	*tmp;

	if (!node || !node->cmds)
		d_throw_error("expand_variable_handler", "node or cmds is null");//unexpected error
	i = 0;
	while (node->cmds[i])
	{
		if (ft_strchr(node->cmds[i], DOLLAR))
		{
			tmp = NULL;
			tmp = expand_variable(node->cmds[i], ctx);
			free(node->cmds[i]);
			node->cmds[i] = NULL;
			node->cmds[i] = tmp;
		}
		i++;
	}
	if (node->left && node->left->fd_num != -1)
		node->left->fd_num = expand_heredoc_var(node->left, ctx);
}
/*
Remove quote handler should remove quotation 
which is not surrounded by other quotation.
e.g. 
"Hello" -> Hello
"'Hello'" -> 'Hello'
'Hello'"hello" -> Hellohello
*/

void	remove_quote_handler(t_node *node)
{
	int		i;
	char	*tmp;

	if (!node || !node->cmds)
		d_throw_error("remove_quote_handler", "node or cmds is null");
	i = 0;
	while (node->cmds[i])
	{
		if (ft_strchr(node->cmds[i], '\'') || ft_strchr(node->cmds[i], '\"'))
		{
			tmp = NULL;
			tmp = remove_quotes(node->cmds[i]);
			free(node->cmds[i]);
			node->cmds[i] = NULL;
			node->cmds[i] = tmp;
		}
		i++;
	}
}
