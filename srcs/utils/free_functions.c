/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 18:05:18 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/22 20:37:23 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "../../includes/utils.h"

void	free_node(t_node *node)
{
	if (!node)
		return ;
	if (node->left)
		free_node(node->left);
	if (node->right)
		free_node(node->right);
	if (node->cmds)
		free_wordlist(node->cmds);
	free(node);
}

void	free_wordlist(char **wordlist)
{
	int	i;

	if (!wordlist)
		return ;
	i = 0;
	while (wordlist[i])
	{
		free(wordlist[i]);
		i++;
	}
	free(wordlist);
}

void	free_ast(t_node **node)
{
	if (!node || !*node)
		return ;
	free_ast(&(*node)->left);
	free_ast(&(*node)->right);
	if ((*node)->cmds)
	{
		for (int i = 0; (*node)->cmds[i]; i++)
			free((*node)->cmds[i]);
		free((*node)->cmds);
		(*node)->cmds = NULL;
	}
	if ((*node)->redirects)
	{
		for (int i = 0; (*node)->redirects[i]; i++)
		{
			free((*node)->redirects[i]);
			(*node)->redirects[i] = NULL;
		}
		free((*node)->redirects);
		(*node)->redirects = NULL;
	}
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
