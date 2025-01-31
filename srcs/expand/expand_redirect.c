/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 01:46:58 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/02/01 02:09:09 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../libft/libft.h"
#include "../../includes/expand.h"
#include "../../includes/expand_asterisk.h"

void	expand_var_rd_handler(t_node *node, t_context *ctx)
{
	int		i;
	char	*tmp;

	if (!node || !node->redirects)
		throw_unexpected_error("expand_var_rd_handler", "null err");
	i = 0;
	while (node->redirects[i])
	{
		if (ft_strchr(node->redirects[i], DOLLAR))
		{
			tmp = NULL;
			tmp = expand_variable(node->redirects[i], ctx);
			free(node->redirects[i]);
			node->redirects[i] = NULL;
			node->redirects[i] = tmp;
		}
		i++;
	}
}

void	expand_aster_rd_handler(t_node *node)
{
	size_t	i;
	char	**tmp;

	if (!node || !node->redirects)
		throw_unexpected_error("expand_aster_rd_handler", NULL);
	i = 0;
	while (node->redirects[i])
	{
		if (ft_strchr(node->redirects[i], ASTERISK))
		{
			tmp = NULL;
			tmp = expand_asterisk(node->redirects[i]);
			if (tmp)
			{
				i = i + recreate_redirect_list(node, tmp, i);
				continue ;
			}
		}
		i++;
	}
}

void	remove_quote_rd_handler(t_node *node)
{
	int		i;
	char	*tmp;

	if (!node || !node->redirects)
		throw_unexpected_error("remove_quote_handler", "arg is null");
	i = 0;
	while (node->redirects[i])
	{
		if (ft_strchr(node->redirects[i], '\'') \
		|| ft_strchr(node->redirects[i], '\"'))
		{
			tmp = NULL;
			tmp = remove_quotes(node->redirects[i]);
			free(node->redirects[i]);
			node->redirects[i] = NULL;
			node->redirects[i] = tmp;
		}
		i++;
	}
}

size_t	recreate_redirect_list(t_node *node, char **file_arry, size_t i)
{
	size_t	ret;
	size_t	origin_arry_size;
	size_t	new_arry_size;
	char	**new_rds;

	ret = 0;
	if (!node || !file_arry)
		return (ret);
	origin_arry_size = get_char_arry_size(node->redirects);
	new_arry_size = get_char_arry_size(file_arry);
	new_rds = xmalloc(sizeof(char *) * (origin_arry_size + new_arry_size));
	ret = pack_new_cmds(node->redirects, file_arry, new_rds, i);
	free_wordlist(&node->redirects);
	free_wordlist(&file_arry);
	node->redirects = NULL;
	node->redirects = new_rds;
	return (ret);
}
