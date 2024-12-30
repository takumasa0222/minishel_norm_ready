/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 18:05:18 by ssoeno            #+#    #+#             */
/*   Updated: 2024/12/30 23:10:13 by ssoeno           ###   ########.fr       */
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
