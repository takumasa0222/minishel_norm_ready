/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookup_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:37:41 by ssoeno            #+#    #+#             */
/*   Updated: 2024/10/31 21:40:01 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_builtin	*lookup_builtin(char *cmd)
{
	t_builtin	builtins_list[4];
	t_builtin	*p;

	builtins_list[0] = (t_builtin){"cd", builtin_cd};
	builtins_list[1] = (t_builtin){"pwd", builtin_pwd};
	builtins_list[2] = (t_builtin){"exit", builtin_exit};
	builtins_list[3] = (t_builtin){NULL, NULL};
	p = builtins_list;
	while (p->name)
	{
		if (ft_strncmp(cmd, p->name, ft_strlen(p->name)) == 0)
			return (p);
		p++;
	}
	return (NULL);
}