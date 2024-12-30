/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:49:31 by ssoeno            #+#    #+#             */
/*   Updated: 2024/11/23 16:53:22 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/builtin.h"
#include "../../includes/utils.h"

int	builtin_env(int argc, char *argv[], t_map *envmap)
{
	t_item	*cur;

	(void) argv;
	if (argc > 1)
	{
		builtin_error("env", NULL, "Arguments are not supproted");
		return (EXIT_FAILURE);
	}
	cur = envmap->item_head.next;
	while (cur)
	{
		if (cur->value)
			printf("%s=%s\n", cur->name, cur->value);
		cur = cur->next;
	}
	printf("_=/usr/bin/env\n");
	return (EXIT_SUCCESS);
}
/*
print "_=/usr/bin/env" to emulate 
the behavior of the env command.
shell commands are executed with _ set to the path of the command
so that the command can know its own path
echo $_ prints the path of the command
*/
