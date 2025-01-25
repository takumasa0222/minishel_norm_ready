/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 18:05:18 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/25 15:47:20 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../includes/utils.h"

void	free_ctx(t_context **ctx)
{
	if (!ctx)
		return ;
	if (*ctx)
	{
		if ((*ctx)->in_pipe_fd > 2)
			close((*ctx)->in_pipe_fd);
		if ((*ctx)->out_pipe_fd > 2)
			close((*ctx)->out_pipe_fd);
		if ((*ctx)->pre_in_pipe_fd > 2)
			close((*ctx)->pre_in_pipe_fd);
		if ((*ctx)->stored_stdin > 2)
			close((*ctx)->stored_stdin);
		if ((*ctx)->stored_stdout > 2)
			close((*ctx)->stored_stdout);
		free_map(&(*ctx)->env);
		free_ast((*ctx)->head_node);
		free(*ctx);
	}
}

void	free_wordlist(char ***wordlist)
{
	int	i;

	if (!wordlist)
		return ;
	i = 0;
	while ((*wordlist) && (*wordlist)[i])
	{
		free((*wordlist)[i]);
		i++;
	}
	free(*wordlist);
}

void	free_ast(t_node **node)
{
	char	**cmds;
	char	**redirects;

	if (!node || !*node)
		return ;
	free_ast(&(*node)->left);
	free_ast(&(*node)->right);
	cmds = (*node)->cmds;
	while (cmds && *cmds)
		free(*cmds++);
	redirects = (*node)->redirects;
	while (redirects && *redirects)
		free(*redirects++);
	free((*node)->cmds);
	(*node)->cmds = NULL;
	free((*node)->redirects);
	(*node)->redirects = NULL;
	free(*node);
	*node = NULL;
}

void	free_token_list(t_token *token_list)
{
	t_token	*temp;

	while (token_list)
	{
		temp = token_list->next;
		if (token_list->word)
		{
			free(token_list->word);
			token_list->word = NULL;
		}
		free(token_list);
		token_list = temp;
	}
}

void	free_map(t_map **map)
{
	t_item	*cur;
	t_item	*next;

	if (!map || !*map)
		return ;
	cur = (*map)->item_head.next;
	while (cur)
	{
		next = cur->next;
		free(cur->name);
		free(cur->value);
		free(cur);
		cur = next;
	}
	free(*map);
}
