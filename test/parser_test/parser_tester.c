/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 02:16:13 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/21 18:50:52 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./parser_tester.h"


int	main(void)
{
	exec_parser_test();
	return (0);
}

void	exec_parser_test(void)
{
	t_token	*token_list;
	t_node	*astree;
	int		i;

	i = 0;
	char *test_words[][30] = {
    {"ls"}, // テストケース1
    {">","file"},   // テストケース2
    {"cat", "|", "abc"},       // テストケース3
	{"(","cat",")"},	// テストケース4
	{"<","file"},		// テストケース5
	{"<<", "end"},		// テストケース6
	{">>","file"},		// テストケース7
	{"cat", "&&", "ls"},// テストケース8
	{"cat", "||", "abc"},// テストケース9
	{"(","cat", "||", "abc",")"},	// テストケース10
	{"cat", ">", "file", "|", "grep", "abc"},       // テストケース11
	{"(", "cat", "&&", "ls", "|", "grep", "abc", ")"}, // テストケース12
	{"echo", "Hello", ">", "output.txt"},           // テストケース13
	{"(", "ls", "||", "cat", ">", "file", ")"},      // テストケース14
	{"echo", "abc", "&&", "ls", "||", "cat"},        // テストケース15
	{"echo", "Hello", ">", "file", "&&", "cat"},     // テストケース16
	{"(", "echo", "Hello", ">", "file", "&&", "cat", ")"}, // テストケース17
	{"(", "echo", "abc", "&&", "ls", "|", "grep", "xyz", ")"}, // テストケース18
	{"ls", "|", "grep", "abc", "||", "cat"},        // テストケース19
	{"cat", ">", "file", "||", "ls"},                 // テストケース20
	{"(", "echo", "Hello", ">", "file", "&&", "(", "cat", "|", "grep", "abc", ")", "||", "ls", ")"},  // テストケース21
	{"echo", "abc", ">", "file", "||", "(", "ls", "|", "grep", "xyz", ")", "&&", "cat"},  // テストケース22
	{"(", "cat", ">", "file", "|", "grep", "abc", ")", "&&", "ls", "|", "sort"},  // テストケース23
	{"echo", "start", ">", "file1", "&&", "(", "echo", "middle", "|", "sort", ">", "file2", ")", "||", "echo", "end"},  // テストケース24
	{"(", "(", "ls", "|", "grep", "abc", ")", "&&", "echo", "done", ">", "output", ")", "||", "cat"},  // テストケース25
	{"echo", "foo", ">", "file1", "&&", "(", "echo", "bar", ">", "file2", "&&", "cat", ")", "|", "sort"},  // テストケース26
	{"(", "echo", "foo", "|", "sort", ">", "file1", "&&", "(", "ls", "|", "grep", "bar", ")", "||", "cat", ")"},  // テストケース27
	{"cat", ">", "file", "|", "(", "echo", "start", "|", "grep", "xyz", ")", "&&", "ls", "|", "sort"},  // テストケース28
	{"(", "echo", "first", "&&", "ls", ">", "file1", "|", "cat", "&&", "echo", "second", ")", "||", "cat"},  // テストケース29
	{"(", "echo", "start", ">", "file", "&&", "(", "cat", "|", "grep", "abc", ")", "&&", "echo", "end", ")", "||", "ls"},  // テストケース30
	{"cat", "|", "abc", "|", "echo", "|", "def"},  // テストケース31
	{"pwd", "||", "ls", "||", "date"}, // テストケース32 soeno テスト1
	{"pwd", "&&", "ls", "&&", "date"}, // テストケース33 soeno テスト2
	{"(", "ls", "|", "wc", "-l", ">", "file1", ")", "||", "echo", "success"}, // テストケース34 soeno テスト3	
	{"echo", "hello", ">", "outfile1", ">", "outfile2"} // テストケース35 soeno テスト4
};

	t_node_kind test_kinds[][30] = {
    {ND_CMD},      // テストケース1
    {ND_REDIRECTS, ND_CMD},      // テストケース2
    {ND_CMD, ND_PIPE, ND_CMD},       // テストケース3
	{ND_L_PARE, ND_CMD, ND_R_PARE},			// テストケース4
	{ND_REDIRECTS, ND_CMD},			// テストケース5
	{ND_REDIRECTS, ND_CMD},			// テストケース6
	{ND_REDIRECTS, ND_CMD},			// テストケース7
	{ND_CMD, ND_AND_OP, ND_CMD},			// テストケース8
	{ND_CMD, ND_OR_OP, ND_CMD},			// テストケース9
	{ND_L_PARE, ND_CMD,ND_OR_OP,ND_CMD, ND_R_PARE},				// テストケース10
	{ND_CMD, ND_REDIRECTS, ND_CMD, ND_PIPE, ND_CMD, ND_CMD},         // テストケース11
	{ND_L_PARE, ND_CMD, ND_AND_OP, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE}, // テストケース12
	{ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD},                             // テストケース13
	{ND_L_PARE, ND_CMD, ND_OR_OP, ND_CMD, ND_REDIRECTS, ND_CMD, ND_R_PARE},  // テストケース14
	{ND_CMD, ND_CMD, ND_AND_OP, ND_CMD, ND_OR_OP, ND_CMD},           // テストケース15
	{ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_AND_OP, ND_CMD},      // テストケース16
	{ND_L_PARE, ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_AND_OP, ND_CMD, ND_R_PARE}, // テストケース17
	{ND_L_PARE, ND_CMD, ND_CMD, ND_AND_OP, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE}, // テストケース18
	{ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_OR_OP, ND_CMD},             // テストケース19
	{ND_CMD, ND_REDIRECTS, ND_CMD, ND_OR_OP, ND_CMD},                    // テストケース20
	{ND_L_PARE, ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_AND_OP, ND_L_PARE, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE, ND_OR_OP, ND_CMD, ND_R_PARE}, // テストケース21
	{ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_OR_OP, ND_L_PARE, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE, ND_AND_OP, ND_CMD}, // テストケース22
	{ND_L_PARE, ND_CMD, ND_REDIRECTS, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE, ND_AND_OP, ND_CMD, ND_PIPE, ND_CMD}, // テストケース23
	{ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_AND_OP, ND_L_PARE,ND_CMD,ND_CMD, ND_PIPE, ND_CMD, ND_REDIRECTS, ND_CMD, ND_R_PARE, ND_OR_OP, ND_CMD, ND_CMD}, // テストケース24
	{ND_L_PARE, ND_L_PARE, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE, ND_AND_OP, ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_R_PARE, ND_OR_OP, ND_CMD}, // テストケース25
	{ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_AND_OP, ND_L_PARE, ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_AND_OP, ND_CMD, ND_R_PARE, ND_PIPE, ND_CMD}, // テストケース26
	{ND_L_PARE, ND_CMD, ND_CMD, ND_PIPE, ND_CMD, ND_REDIRECTS, ND_CMD, ND_AND_OP, ND_L_PARE, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE, ND_OR_OP, ND_CMD,ND_R_PARE}, // テストケース27
	{ND_CMD, ND_REDIRECTS, ND_CMD, ND_PIPE, ND_L_PARE, ND_CMD, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE, ND_AND_OP, ND_CMD, ND_PIPE, ND_CMD}, // テストケース28
	{ND_L_PARE, ND_CMD, ND_CMD, ND_AND_OP, ND_CMD, ND_REDIRECTS, ND_CMD, ND_PIPE, ND_CMD, ND_AND_OP, ND_CMD, ND_R_PARE, ND_OR_OP, ND_CMD}, // テストケース29
	{ND_L_PARE, ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_AND_OP, ND_L_PARE, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_R_PARE, ND_AND_OP, ND_CMD,ND_CMD, ND_R_PARE, ND_OR_OP, ND_CMD}, // テストケース30
    {ND_CMD, ND_PIPE, ND_CMD, ND_PIPE, ND_CMD, ND_PIPE, ND_CMD},
	{ND_CMD, ND_OR_OP, ND_CMD, ND_OR_OP, ND_CMD}, // テストケース32 soeno テスト1
	{ND_CMD, ND_AND_OP, ND_CMD, ND_AND_OP, ND_CMD}, // テストケース33 soeno テスト2
	{ND_L_PARE, ND_CMD, ND_PIPE, ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_R_PARE, ND_OR_OP, ND_CMD, ND_CMD}, // テストケース34 ssoeno テスト3
	{ND_CMD, ND_CMD, ND_REDIRECTS, ND_CMD, ND_REDIRECTS, ND_CMD} // テストケース34 ssoeno テスト3	
	};
	int	list_size[35] = {
		1,
		2,
		3,
		3,
		2,
		2,
		2,
		3,
		3,
		5,
		6, //case11
		8, //case12
		4, //case13
		7, //case14
		6, //case15
		6, //case16
		8, //case17
		9, //case18
		6, //case19
		5, //case20
		15, //case21
		13, //case22
		12, //case23
		16, //case24
		15, //case25
		15, //case26
		17, 
		14, 
		13, 
		18,
		7,
		5,
		5,
		11,
		6
	};

	for (int i = 0; i < 35; i++)
	{
		printf("---------------testcase [%d]\n",i + 1);
		token_list = create_dummy_token_list(test_words[i], test_kinds[i], list_size[i]);
		if (!token_list)
			printf("unexpexted error in exec_parser_test");
		astree = parse_cmd(&token_list);
		print_tree(astree, 0, "root");
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
	printf("%s (%s)\n", node_kind_conv(node->kind), relation);
	// cmds や redirects の中身を確認したい場合はコメントアウト外す
	// for (int i = 0; i < 10; i++)
	// {
	// 	if (!node->cmds)
	// 		break;
	// 	if (node->cmds[i])
	// 		printf("cmds[%d]: %s\n",i, node->cmds[i]);
	// 	else
	// 		break;
	// }
	// for (int i = 0; i < 10; i++)
	// {
	// 	if (!node->redirects)
	// 		break;
	// 	if (node->redirects[i])
	// 		printf("redirects[%d]: %s\n",i, node->redirects[i]);
	// 	else
	// 		break;
	// }
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
