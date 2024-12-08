/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comb_exec_tester.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:45:44 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/08 19:00:17 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMB_EXEC_TESTER_H
# define COMB_EXEC_TESTER_H

# include <fcntl.h>
# include <stdio.h>
# include "../../includes/utils.h"
# include "../../includes/parser.h"
# include "../../includes/lexer.h"
# include "../../includes/minishell.h"
# include "../../includes/execute.h"

int	main(int argc, char *argv[], char *envp[]);
void	exec_combination_test(char **envp);
t_token	*create_dummy_token_list(char **word_list, t_node_kind *kind_list, int list_size);
char	*node_kind_conv(t_node_kind node_kind);
t_token	*create_tokens(char *word, t_node_kind kind);
void	print_tree(t_node *node, int depth, char *relation);
void temp_starter(t_node *ast_node, char **envp);
#endif