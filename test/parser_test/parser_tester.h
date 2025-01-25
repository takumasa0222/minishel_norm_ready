/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tester.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 02:18:54 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/23 04:36:51 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_TESTER_H
# define PARSER_TESTER_H

# include <fcntl.h>
# include "../lexer_test/ft_get_next_line/get_next_line_bonus.h"
# include <stdio.h>
# include "../../includes/utils.h"
# include "../../includes/parser.h"
# include "../../includes/lexer.h"
# include "../../includes/minishell.h"

# define DEFAULT_PARSER_TEST_CASE "parser_testcase.txt"
# define NUMBER_OF_TESTS 3

int	main(void);
void	exec_parser_test(void);
t_token	*create_dummy_token_list(char **word_list, t_node_kind *kind_list, int list_size);
char	*node_kind_conv(t_node_kind node_kind);
t_token	*create_tokens(char *word, t_node_kind kind);
void	print_tree(t_node *node, int depth, char *relation);
void	run_test_parser();
#endif