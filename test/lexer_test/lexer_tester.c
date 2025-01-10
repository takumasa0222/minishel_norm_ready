/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:10:27 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/10 21:06:07 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_tester.h"

/*
void test_lexer(const char *input)
{
    t_token *tokens;
    t_token *current;

    if (!input)
    {
        printf("Input is NULL\n");
        return;
    }

    printf("Testing lexer with input: \"%s\"\n", input);
    tokens = lexer((char *)input);

    current = tokens;
    while (current && current->kind != ND_EOF)
    {
        printf("Token: word=\"%s\", kind=%d\n", current->word, current->kind);
        current = current->next;
    }
}

int main(void)
{
    test_lexer("< infile cat -e | wc -l > outfile");
    return 0;
}
*/

int	main(int argc, char *argv[])
{
	if (argc == 1)
		exec_lexer_test(NULL, NULL);
	else if (argc == 3)
		exec_lexer_test(argv[1], argv[2]);
	return (0);
}

void	exec_lexer_test(char *test_file_name, char *expected_file)
{
	t_token	*ret;
	int		fd;
	int		fd2;
	int		i;
	char	*test;
	char	*expected;

	if (!test_file_name || !expected_file)
	{
		fd = open(DEFAULT_TEST, O_RDONLY);
		fd2 = open(EXPECTED_RESULT, O_RDONLY);
	}
	else
	{
		fd = open(test_file_name, O_RDONLY);
		fd2 = open(expected_file, O_RDONLY);
	}
	if (fd < 0 || fd2 < 0)
		exit(EXIT_FAILURE);
	i = 1;
	while ((test = get_next_line(fd)))
	{
		ret = lexer(test);
		expected = get_next_line(fd2);
		//node_printer(ret);
		if (!match(ret, expected))
			printf("testcase %d NG\n", i);
		else
			printf("testcase %d OK\n", i);
		i++;
		free(test);
		free(expected);
		test = NULL;
		expected = NULL;
		free_token_list(ret);
		ret = NULL;
	}
}

bool	match(t_token *token, char *str)
{
	t_token		*copy_token;
	char 		**str_list;
	int			i;

	str_list = ft_split(str,',');
	copy_token = token;
	if (!token)
		printf("token is NULL");
	i = 0;
	while (copy_token->kind != ND_EOF && copy_token->word)
	{
		if (ft_strncmp(copy_token->word, str_list[i], ft_strlen(str_list[i])))
		{
			free_words(str_list);
			return	(false);
		}
		copy_token = copy_token->next;
		i++;
	}
	if (ft_strncmp(str_list[i],"\n",1) || str_list[i + 1])
		return (false);
	free_words(str_list);
	return (true);
}

void	free_token_list(t_token **token_list)
{
	t_token	*temp;

	if (!token_list || !*token_list)
		return;
	while (*token_list)
	{
		temp = (*token_list)->next;
		if ((*token_list)->word)
		{
			free((*token_list)->word);
			(*token_list)->word = NULL;
		}
		free((*token_list));
		(*token_list) = temp;
	}
}
void	free_words(char	**word)
{
	int	i;

	i = 0;
	if (!word)
		return ;
	while (word[i])
	{
		free(word[i]);
		i++;
	}
	free(word);
}


void	node_printer(t_token *token)
{
	static char	*token_kind[] = {
		"TK_WORD",
		"TK_RESERVED",
		"TK_OP",
		"TK_EOF"
	};
	t_token		*copy_token;

	copy_token = token;
	if (!token)
		printf("token is NULL");
	while (copy_token->kind != ND_EOF && copy_token->word)
	{
		printf("word: %s, kind: %s\n", copy_token->word, token_kind[copy_token->kind]);
		copy_token = copy_token->next;
	}
}
