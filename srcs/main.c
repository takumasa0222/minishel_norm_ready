/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/08 22:16:23 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"

void print_tree(t_node *node, int depth, char *relation);

int	main(void)
{
	char	*line;
	t_token	*token_list;
	t_node	*ast;

	rl_outstream = stderr;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
		{
			add_history(line);
			token_list = lexer(line);
			ast = parse_cmd(&token_list);
			print_tree(ast, 0, "root");
		}
		free(line);
	}
	exit(0);
}

void	print_tree(t_node *node, int depth, char *relation)
{
	if (node == NULL) 
	{
		return;
	}
	for (int i = 0; i < depth; i++)
	{
		printf("    ");
	}
	// 現在のノードと親子関係を表示
	printf("%u (%s)\n", node->kind, relation);
	// cmds や redirects の中身を確認したい場合はコメントアウト外す
	for (int i = 0; i < 10; i++)
	{
		if (!node->cmds)
			break;
		if (node->cmds[i])
			printf("cmds[%d]: %s\n",i, node->cmds[i]);
		else
			break;
	}
	for (int i = 0; i < 10; i++)
	{
		if (!node->redirects)
			break;
		if (node->redirects[i])
			printf("redirects[%d]: %s\n",i, node->redirects[i]);
		else
			break;
	}
	// 左右の子を再帰的に表示
	if (node->left)
	{
		print_tree(node->left, depth + 1, "left child");
	}
	if (node->right)
	{
		print_tree(node->right, depth + 1, "right child");
	}
}