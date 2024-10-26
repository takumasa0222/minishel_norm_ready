/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:10:27 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/10/26 20:43:54 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_tester.h"

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
		printf("testval %s\n", test);
		ret = lexer(test);
		expected = get_next_line(fd2);
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
	while (copy_token->kind != TK_EOF && copy_token->word)
	{
		if (copy_token->word != str_list[i])
			return	(false);
		copy_token = copy_token->next;
		i++;
	}
	return (true);
}

void	free_token_list(t_token	*token)
{
	t_token	*temp;


	if (!token)
		return ;
	while (token)
	{
		temp = token->next;
		free(token);
		token = NULL;
		token = temp;
	}
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
	while (copy_token->kind != TK_EOF && copy_token->word)
	{
		printf("word: %s, kind: %s\n", copy_token->word, token_kind[copy_token->kind]);
		copy_token = copy_token->next;
	}
}