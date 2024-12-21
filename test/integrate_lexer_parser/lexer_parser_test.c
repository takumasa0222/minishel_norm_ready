#include "../../includes/minishell.h"
#include "../lexer_test/lexer_tester.h"
#include "../parser_test/parser_tester.h"

char *test_input[] = {
    "ls",                      // テストケース1
    "> file",                  // テストケース2
    "cat | abc",               // テストケース3
    "( cat )",                 // テストケース4
    "< file",                  // テストケース5
    "<< end",                  // テストケース6
    ">> file",                 // テストケース7
    "cat && ls",               // テストケース8
    "cat || abc",              // テストケース9
    "( cat || abc )",          // テストケース10
    "cat > file | grep abc",   // テストケース11
    "( cat && ls | grep abc )",// テストケース12
    "echo Hello > output.txt", // テストケース13
    "( ls || cat > file )",    // テストケース14
    "echo abc && ls || cat",   // テストケース15
    "echo Hello > file && cat",// テストケース16
    "( echo Hello > file && cat )", // テストケース17
    "( echo abc && ls | grep xyz )", // テストケース18
    "ls | grep abc || cat",    // テストケース19
    "cat > file || ls",        // テストケース20
    "( echo Hello > file && ( cat | grep abc ) || ls )", // テストケース21
    "echo abc > file || ( ls | grep xyz ) && cat",      // テストケース22
    "( cat > file | grep abc ) && ls | sort",          // テストケース23
    "echo start > file1 && ( echo middle | sort > file2 ) || echo end", // テストケース24
    "(( ls | grep abc ) && echo done > output ) || cat", // テストケース25
    "echo foo > file1 && ( echo bar > file2 && cat ) | sort", // テストケース26
    "( echo foo | sort > file1 && ( ls | grep bar ) || cat )", // テストケース27
    "cat > file | ( echo start | grep xyz ) && ls | sort", // テストケース28
    "( echo first && ls > file1 | cat && echo second ) || cat", // テストケース29
    "( echo start > file && ( cat | grep abc ) && echo end ) || ls", // テストケース30
    "cat | abc | echo | def",  // テストケース31
    "pwd || ls || date",       // テストケース32
    "pwd && ls && date",       // テストケース33
    "( ls | wc -l > file1 ) || echo success", // テストケース34
	"echo hello > outfile1 > outfile2" // テストケース35
};

void print_tree(t_node *node, int depth, char *relation);
void free_ast(t_node **node);
void free_token_list(t_token *token_list);

int	main(void)
{
	t_token	*token_list;
	t_node	*ast;

	int i = 0;
	while(i < 35)
	{
		printf("--------------- Testcase [%d] ---------------\n", i + 1);
		token_list = lexer(test_input[i]);
		ast = parse_cmd(&token_list);
		print_tree(ast, 0, "root");
		free_ast(&ast);
		free_token_list(token_list);
		i++;
	}
	exit(0);
}

char	*node_kind_conv(t_node_kind node_kind)
{
	char	*node_list[] = {
		"REDI",
		// "IN_R",
		// "OUT_",
		// "APPE",
		// "HERE",
		"PIPE",
		"OR_O",
		"AND_",
		"L_PA",
		"R_PA",
		"SUBS",
		"CMD",
		"FD_N",
		// "WORD"
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
	if (node->cmds)
    {
		printf(" [cmds: ");
		for (int i = 0; node->cmds[i]; i++)
		{
			printf("%s ", node->cmds[i]);
			printf(" ");
		};
        printf("]");
    }
	if (node->op_val) {
		printf(" [op_val: %s]", node->op_val);
	}
	if (node->fd_num) {
		printf(" [fd_num: %d]", node->fd_num);
	}
	if (node->redirects)
	{
		printf(" [redirects: ");
		for (int i = 0; node->redirects[i]; i++)
		{
			printf("%s ", node->redirects[i]);
			if (!node->redirects[i + 1])
				printf(", ");
		};
		printf("]");
	}
	printf("\n");
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

void free_ast(t_node **node)
{
    if (!node || !*node)
        return;
    free_ast(&(*node)->left);
    free_ast(&(*node)->right);

    // // node->cmdsの解放
    if ((*node)->cmds)
    {
        for (int i = 0; (*node)->cmds[i]; i++)
            free((*node)->cmds[i]); // cmds[i]自体が動的確保されているなら
        free((*node)->cmds);
		(*node)->cmds = NULL;
    }

    // // // node->redirectsの解放
    if ((*node)->redirects)
    {
        for (int i = 0; (*node)->redirects[i]; i++)
            free((*node)->redirects[i]);
        free((*node)->redirects);
		(*node)->redirects = NULL;
    }

    free(*node);
    *node = NULL;
}

void free_token_list(t_token *token_list)
{
    t_token *temp;
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
