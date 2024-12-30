/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:36:46 by shokosoeno        #+#    #+#             */
/*   Updated: 2024/12/30 19:18:48 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"
#include "../includes/execute.h"
#include "../includes/utils.h"
void	print_tree(t_node *node, int depth, char *relation);

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	t_token	*token_list;
	t_node	*ast;

	rl_outstream = stderr;
	setup_signal();
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
			free_node(ast);
			if (token_list)
				free_token_list(token_list);
		}
		free(line);
	}
	exit(0);
}

char	*node_kind_conv(t_node_kind node_kind)
{
	char	*node_list[9];

	node_list[0] = "REDI";
	// node_list[1] = "IN_R";
	// node_list[2] = "OUT_";
	// node_list[3] = "APPE";
	// node_list[4] = "HERE";
	node_list[1] = "PIPE";
	node_list[2] = "OR_O";
	node_list[3] = "AND_";
	node_list[4] = "L_PA";
	node_list[5] = "R_PA";
	node_list[6] = "SUBS";
	node_list[7] = "CMD";
	node_list[8] = "FD_N";
	// node_list[9] = "WORD";
	return (node_list[node_kind]);
}

void	print_tree(t_node *node, int depth, char *relation)
{
	int	i;

	if (node == NULL)
		return ;
	i = 0;
	while (i++ < depth)
		printf("    ");
	printf("%s (%s)\n", node_kind_conv(node->kind), relation);
	if (node->left)
	{
		print_tree(node->left, depth + 1, "left child");
	}
	if (node->right)
	{
		print_tree(node->right, depth + 1, "right child");
	}
}

char	*node_kind_conv(t_node_kind node_kind)
{
	char	*node_list[9];

	node_list[0] = "REDI";
	// node_list[1] = "IN_R";
	// node_list[2] = "OUT_";
	// node_list[3] = "APPE";
	// node_list[4] = "HERE";
	node_list[1] = "PIPE";
	node_list[2] = "OR_O";
	node_list[3] = "AND_";
	node_list[4] = "L_PA";
	node_list[5] = "R_PA";
	node_list[6] = "SUBS";
	node_list[7] = "CMD";
	node_list[8] = "FD_N";
	// node_list[9] = "WORD";
	return (node_list[node_kind]);
}

void	print_tree(t_node *node, int depth, char *relation)
{
	int	i;

	if (node == NULL)
		return ;
	i = 0;
	while (i++ < depth)
		printf("    ");
	printf("%s (%s)\n", node_kind_conv(node->kind), relation);
	if (node->left)
	{
		print_tree(node->left, depth + 1, "left child");
	}
	if (node->right)
	{
		print_tree(node->right, depth + 1, "right child");
	}
}

//void	start_exec(char *line, char **envp)
//{
//	t_token		*token_list;
//	t_node		*ast_node;

//	token_list = lexer(line);
//	ast_node = parse_cmd(&token_list);
//	ctx = init_ctx();
//	exec_handler(ast_node, envp, ctx);
//	if (ctx->cnt)
//		wait_children_status(ctx);
//}