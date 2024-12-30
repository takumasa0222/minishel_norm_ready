/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 22:55:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/18 01:36:56 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./expand_tester.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*ret;

	ret = NULL;
	if (!argv || argc < 0)
		return (1);

	if (argv[1])
	{
		ret = expand_variable(argv[1], envp);
		printf("result %s\n",ret);
	}
	else
		test_expand_variable(envp);
}

void	test_expand_variable(char *envp[])
{
	char	*ret;

	ret = NULL;
	char	*test_cases[] ={
		"$USER",
		"'$USER'",
		"$USER$HOME",
		"$USERA",
		"$USER'A'",
		"$USER\"A\"",
		"$USER*",
		"$USER$123",
		"$USER$",
		"$USER$$USER",
		"$USE'R'",
		"'$USER'",
		"'$USER'\"USER\"\"$USER\"",
		"'   $USER   '\"   '$USER'  \"",
		"'$'USER\"$\"USER",
		NULL
	};

	for (int i = 0; test_cases[i]; i++)
	{
		ret = expand_variable(test_cases[i], envp);
		printf("result %s\n",ret);
		free(ret);
		ret = NULL;
	}
}
