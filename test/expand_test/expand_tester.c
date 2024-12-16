/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 22:55:35 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/16 23:16:45 by tamatsuu         ###   ########.fr       */
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
		printf("result %s\n",argv[1]);
		ret = expand_variable(argv[1], envp);
	}
	printf("result %s\n",ret);
}
