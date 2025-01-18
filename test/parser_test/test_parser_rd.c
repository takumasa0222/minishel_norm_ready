/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser_rd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 01:57:43 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/11 17:08:30 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./parser_tester.h"

int main()
{
	run_test_parser();
}

void	run_test_parser()
{
	t_token	*token_list;
	t_node	*ast_tree;

	char *testcase[] = 
	{
		"ls",  // テストケース1
    	">file",    // テストケース2
    	"cat | abc",        // テストケース3
		"(cat)", 	// テストケース4
		"<file", 		// テストケース5
		"<< end", 		// テストケース6
		">>file",		// テストケース7
		"cat && ls", // テストケース8
		"cat || abc", // テストケース9
		"(cat || abc)", 	// テストケース10
		"cat > file | grep abc",        // テストケース11
		"( cat && ls | grep abc )",  // テストケース12
		"echo Hello > output.txt",            // テストケース13
		"( ls || cat > file )",       // テストケース14
		"echo abc && ls || cat",         // テストケース15
		"echo Hello > file && cat",      // テストケース16
		"( echo Hello > file && cat )",  // テストケース17
		"( echo abc && ls | grep xyz )",  // テストケース18
		"ls | grep abc || cat",         // テストケース19
		"cat > file || ls",                  // テストケース20
		"( echo Hello > file && ( cat | grep abc ) || ls )",   // テストケース21
		"echo abc > file || ( ls | grep xyz ) && cat",   // テストケース22
		"( cat > file | grep abc ) && ls | sort",   // テストケース23
		"echo start > file1 && ( echo middle | sort > file2 ) || echo end",   // テストケース24
		"( ( ls | grep abc ) && echo done > output ) || cat",   // テストケース25
		"echo foo > file1 && ( echo bar > file2 && cat ) | sort",   // テストケース26
		"( echo foo | sort > file1 && ( ls | grep bar ) || cat )",   // テストケース27
		"cat > file | ( echo start | grep xyz ) && ls | sort",   // テストケース28
		"( echo first && ls > file1 | cat && echo second ) || cat",   // テストケース29
		"( echo start > file && ( cat | grep abc ) && echo end ) || ls",   // テストケース30
		"cat | abc | echo | def",   // テストケース31
		"pwd || ls || date",  // テストケース32 soeno テスト1
		"pwd && ls && date",  // テストケース33 soeno テスト2
		"( ls | wc -l > file1 ) || echo success",  // テストケース34 soeno テスト3	
		"echo hello > outfile1 > outfile2",  // テストケース35 soeno テスト4
		"<a <b >c cat Makefile Readme >> eof >> EOF Makefile2 <asdf",
		"<a <b cat Makefile>>eof>>EOF Makefile2",
		">A >B >C >D >E >F>G<H<I<J<K",
		"cat Makefile Readme >> eof >> EOF Makefile2 <asdf",
		">A>>EOF>>EOF",
	};

	token_list = NULL;
	for (int i = 0; i < (int)sizeof(testcase)/(int)sizeof(char*); i++)
	{
		printf("---------------testcase [%d]\n",i + 1);
		token_list = lexer(testcase[i]);
		if (!token_list)
			printf("unexpexted error in exec_parser_test");
		ast_tree = parse_cmd(&token_list);
		print_tree(ast_tree, 0, "root");
		free_ast(&ast_tree);
		ast_tree = NULL;
		printf("---------------testcase [%d] done\n\n",i + 1);
		free_token_list(token_list);
		token_list = NULL;
	}
}
