/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comb_exec_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:45:04 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/11/27 04:22:40 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./comb_exec_tester.h"

int	main(int argc, char *argv[], char *envp[])
{
	if (0 == 1)
		printf("%d, %s \n",argc, argv[0]);
	exec_combination_test(envp);
}

void	exec_combination_test(char **envp)
{
	t_token	*token_list;
	t_node	*astree;
	int		i;

	i = 0;
	char *test_words[][30] = {
	// {"ls"}, // テストケース1
	// {"echo", "hello", "|", "cat"}, // テストケース1
	{"sleep", "2", "|", "cat", "|", "echo", "where is " }, // テストケース1 
	// {"echo", "hello", "|", "cat", "|", "echo", "where is " }, // テストケース1
	// {"echo", "hello", "|", "cat"}, // テストケース1
};

	t_node_kind test_kinds[][30] = {
    // {ND_FD_WORD},      // テストケース1
		// {ND_FD_WORD, ND_FD_WORD, ND_PIPE, ND_FD_WORD},
		{ND_FD_WORD, ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_FD_WORD},
	};
	int	list_size[30] = {
		// 1,
		// 4,
		7
	};

	for (int i = 0; i < 1; i++)
	{
		printf("---------------testcase [%d]\n",i + 1);
		token_list = create_dummy_token_list(test_words[i], test_kinds[i], list_size[i]);
		if (!token_list)
			printf("unexpexted error in exec_parser_test");
		astree = parse_cmd(&token_list);
		exec_handler(astree, envp, 0, false);
		// print_tree(astree, 0, "root");
		astree = NULL;
		printf("---------------testcase [%d] done\n\n",i + 1);
	}
}

t_token	*create_dummy_token_list(char **word_list, t_node_kind *kind_list, int list_size)
{
	t_token	*ret;
	t_token	*tmp;
	int		i;

	i = 0;
	ret = create_tokens(word_list[i], kind_list[i]);
	tmp = ret;
	i++;
	while (i < list_size)
	{
		tmp->next = create_tokens(word_list[i], kind_list[i]);
		tmp = tmp->next;
		i++;
	}
	return (ret);
}


t_token	*create_tokens(char *word, t_node_kind kind)
{
	t_token	*ret;

	ret = malloc(1 * sizeof(t_token));
	if (!ret)
		d_throw_error("create_token", "malloc failed");
	ret->kind = kind;
	ret->word = word;
	ret->next = NULL;
	return (ret);
}

char	*node_kind_conv(t_node_kind node_kind)
{
	char	*node_list[] = {
		"REDI",
		"IN_R",
		"OUT_",
		"APPE",
		"HERE",
		"PIPE",
		"OR_O",
		"AND_",
		"L_PA",
		"R_PA",
		"SUBS",
		"CMD",
		"FD_N",
		"WORD"
	};
	return (node_list[node_kind]);
}
