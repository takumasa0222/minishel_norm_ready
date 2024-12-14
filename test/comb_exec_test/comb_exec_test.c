/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comb_exec_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:45:04 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/15 01:11:08 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./comb_exec_tester.h"

int	main(int argc, char *argv[], char *envp[])
{
	if (0 == 1)
		printf("%d, %s \n",argc, argv[0]);
	exec_combination_test(envp);
}

void temp_starter(t_node *ast_node, char **envp)
{
	t_context	*ctx;

	ctx = init_ctx();
	exec_handler(ast_node, envp, ctx);
	//ctx->last_status = exec_handler(ast_node, envp, ctx);
	if(ctx->cnt)
		wait_children_status(ctx);
	printf("exit_status %d\n",ctx->last_status);
}

void	exec_combination_test(char **envp)
{
	t_token	*token_list;
	t_node	*astree;
	int		i;

	i = 0;
	char *test_words[][30] = {
	{"ls"}, // テストケース0
	{"echo", "hello", "|", "cat"}, // テストケース1
	{"sleep", "1", "|", "cat", "|", "echo", "where is " }, // テストケース2
	{"echo", "hello", "|", "cat", "|", "ls","42" }, // テストケース3
	{"cat", "|", "cat","|", "ls" }, // テストケース4
	{"ls", "||", "ls" }, // テストケース5
	{"false", "||", "ls"}, // テストケース6
	{"true", "||", "ls"}, // テストケース7
	{"false", "||", "ls", "42" }, // テストケース8
	{"false", "||", "ls", "42", "||", "catx" }, // テストケース9
	{"ls", "&&", "ls" }, // テストケース10
	{"false", "&&", "ls"}, // テストケース11
	{"true", "&&", "ls"}, // テストケース12
	{"false", "&&", "ls", "42" }, // テストケース13
	{"false", "&&", "ls", "42", "&&", "catx" }, // テストケース14
	{"false", "||", "ls", "42", "&&", "catx" }, // テストケース15
	{"(","false",")"}, // テストケース16
	{"(","false","|","ls", ")"}, // テストケース17
	{"false","&&","ls", "||", "echo", "hello"},// テストケース18
	{"false","&&","(","ls", "||", "echo", "hello",")"},// テストケース19
	{"false","||","ls", "||", "echo", "hello"},// テストケース20
	{"true","||","(","ls", "||", "echo", "hello",")"},// テストケース21
	{"(","ls", "|", "echo", "first",")","&&","(","false", "||", "echo", "second",")"},// テストケース22
	{"(","sleep", "1", "&&", "echo", "first",")","&&","(","sleep","2", "||", "echo", "second",")"}// テストケース23
	};

	t_node_kind test_kinds[][30] = {
     	{ND_FD_WORD},      // 0
		{ND_FD_WORD, ND_FD_WORD, ND_PIPE, ND_FD_WORD}, //1
		{ND_FD_WORD, ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_FD_WORD}, //2
		{ND_FD_WORD, ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_FD_WORD}, //3
		{ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_PIPE, ND_FD_WORD}, //4
		{ND_FD_WORD, ND_OR_OP, ND_FD_WORD}, //5
		{ND_FD_WORD, ND_OR_OP, ND_FD_WORD}, //6
		{ND_FD_WORD, ND_OR_OP, ND_FD_WORD}, // 7
		{ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD}, //8
		{ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD, ND_OR_OP, ND_FD_WORD}, //9
		{ND_FD_WORD, ND_AND_OP, ND_FD_WORD}, //10
		{ND_FD_WORD, ND_AND_OP, ND_FD_WORD}, //11
		{ND_FD_WORD, ND_AND_OP, ND_FD_WORD}, //12
		{ND_FD_WORD, ND_AND_OP, ND_FD_WORD, ND_FD_WORD}, //13
		{ND_FD_WORD, ND_AND_OP, ND_FD_WORD, ND_FD_WORD, ND_AND_OP, ND_FD_WORD}, //14
		{ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD, ND_AND_OP, ND_FD_WORD}, //15
		{ND_L_PARE, ND_FD_WORD, ND_R_PARE}, //16
		{ND_L_PARE, ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_R_PARE}, //17
		{ND_FD_WORD, ND_AND_OP, ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD}, //18
		{ND_FD_WORD, ND_AND_OP, ND_FD_WORD, ND_L_PARE, ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD, ND_R_PARE}, //19
		{ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD}, //20
		{ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_L_PARE, ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD, ND_R_PARE}, //21
		{ND_L_PARE, ND_FD_WORD, ND_PIPE, ND_FD_WORD, ND_FD_WORD, ND_R_PARE,  ND_AND_OP, ND_L_PARE, ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD, ND_R_PARE}, //22
		{ND_L_PARE, ND_FD_WORD, ND_FD_WORD, ND_AND_OP, ND_FD_WORD, ND_FD_WORD, ND_R_PARE,  ND_AND_OP, ND_L_PARE, ND_FD_WORD,  ND_FD_WORD, ND_OR_OP, ND_FD_WORD, ND_FD_WORD, ND_R_PARE} //23
	};
	int	list_size[30] = {
		1,
		4,
		7,
		7,
		5,
		3,
		3,
		3,
		4,
		6,
		3,
		3,
		3,
		4,
		6,
		6,
		3,
		5,
		6,
		9,
		6,
		9,
		13,
		15
	};

	for (int i = 0; i < 24; i++)
	{
		printf("---------------testcase [%d]\n",i );
		token_list = create_dummy_token_list(test_words[i], test_kinds[i], list_size[i]);
		if (!token_list)
			printf("unexpexted error in exec_parser_test");
		astree = parse_cmd(&token_list);
		temp_starter(astree, envp);

		// print_tree(astree, 0, "root");
		astree = NULL;
		printf("---------------testcase [%d] done\n\n",i);
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
