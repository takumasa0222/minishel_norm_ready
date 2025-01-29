/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_aster_test.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:46:24 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/30 03:09:43 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./expand_aster_test.h"
#include <string.h>
#include <stdlib.h>
const char *cmp_type_to_string(t_cmp_type type);

int	main(void)
{
	test_expand_aster();
}

const char *cmp_type_to_string(t_cmp_type type)
{
    static const char *cmp_type_str[] = {
        "PREFIX",
        "SUFFIX",
        "CONTAIN",
        "ALL"
    };
    return (type >= PREFIX && type <= ALL) ? cmp_type_str[type] : "UNKNOWN";
}

void	test_expand_aster(void)
{
	t_cmp_str	**ret;

	ret = NULL;
	char	*test_cases[] ={
		"*",
		"*.c",
		"abc*",
		"ab*cd",
		"*abcd*",
		"ab*cd*",
		"*ab*cd",
		"a*bc*c",
		"*a*b*c*",
		"*****",
		"***.c",
		"abc****",
		"***abcd***",
		"a****bc****c",
		"***a**b**c****",
		NULL
	};

	for (int i = 0; test_cases[i]; i++)
	{
		ret = create_cmp_str_arry(test_cases[i]);
		printf("test case is: %s\n",test_cases[i]);
		for (int j = 0; ret[j]; j++)
		{
			printf("string:%s\n",ret[j]->compared_str);
			printf("cmp_type:%s\n",cmp_type_to_string(ret[j]->compare_type));
		}
		printf("test case done\n\n");
		free(ret);
		ret = NULL;
	}
}
