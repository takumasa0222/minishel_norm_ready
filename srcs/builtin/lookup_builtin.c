/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookup_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:37:41 by ssoeno            #+#    #+#             */
/*   Updated: 2025/01/19 17:07:34 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../../includes/builtin.h"
#include "../../includes/builtin.h"

t_builtin	*lookup_builtin(char *cmd)
{
	t_builtin			*p;
	static t_builtin	builtins_list[8];

	builtins_list[0] = (t_builtin){"cd", builtin_cd};
	builtins_list[1] = (t_builtin){"pwd", builtin_pwd};
	builtins_list[2] = (t_builtin){"exit", builtin_exit};
	builtins_list[3] = (t_builtin){"export", builtin_export};
	builtins_list[4] = (t_builtin){"unset", builtin_unset};
	builtins_list[5] = (t_builtin){"echo", builtin_echo};
	builtins_list[6] = (t_builtin){"env", builtin_env};
	builtins_list[7] = (t_builtin){NULL, NULL};
	p = builtins_list;
	while (p->name)
	{
		if (ft_strncmp(cmd, p->name, ft_strlen(cmd)) == 0)//CHECK ft_strcmp is better
			return (p);
		p++;
	}
	return (NULL);
}
