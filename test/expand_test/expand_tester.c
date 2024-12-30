/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 22:55:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/31 03:22:27 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./expand_tester.h"
#include <string.h>
#include <stdlib.h>

int	main(int argc, char *argv[], char *envp[])
{
	char	*ret;
	t_map	*env;

	ret = NULL;
	env = init_env(envp);
	if (!argv || argc < 0)
		return (1);

	if (argv[1])
	{
		ret = expand_variable(argv[1], env);
		printf("result %s\n",ret);
	}
	else
		test_expand_variable(env);
}

void	test_expand_variable(t_map *envp)
{
	char	*ret;
	char	*after_ret;

	ret = NULL;
	after_ret = NULL;
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
		if (strchr(ret, '\'') || strchr(ret, '\"'))
		{
			after_ret = remove_quotes(ret);
			printf("result after remove quote: %s\n", after_ret);	
		}
		free(ret);
		free(after_ret);
		ret = NULL;
		after_ret = NULL;
	}
}
