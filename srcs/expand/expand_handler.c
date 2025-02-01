/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 02:57:36 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/02/01 09:37:39 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

//word_split_handler(node);
void	expand_handler(t_node *node, t_context *ctx)
{
	if (!node || !node->cmds)
		throw_unexpected_error("expand_handler", "node or cmds is null");
	expand_variable_handler(node, ctx);
	expand_asterisk_handler(node);
	remove_quote_handler(node);
	if (node && node->left && node->left->redirects)
		expand_redirect_handler(node->left, ctx);
}

void	expand_redirect_handler(t_node *node, t_context *ctx)
{
	expand_var_rd_handler(node, ctx);
	expand_aster_rd_handler(node);
	remove_quote_rd_handler(node);
}

void	expand_variable_handler(t_node *node, t_context *ctx)
{
	int		i;
	char	*tmp;

	if (!node || !node->cmds)
		throw_unexpected_error("expand_variable_handler", "null err");
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

void	expand_asterisk_handler(t_node *node)
{
	size_t	i;
	char	**tmp;

	if (!node || !node->cmds)
		throw_unexpected_error("expand_asterisk_handler", NULL);
	i = 0;
	while (node->cmds[i])
	{
		if (ft_strchr(node->cmds[i], ASTERISK))
		{
			tmp = NULL;
			tmp = expand_asterisk(node->cmds[i]);
			if (tmp)
			{
				i = i + recreate_command_list(node, tmp, i);
				continue ;
			}
		}
		i++;
	}
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
		throw_unexpected_error("remove_quote_handler", "node or cmds is null");
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
