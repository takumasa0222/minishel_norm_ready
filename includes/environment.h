/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 21:12:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2024/12/30 21:34:43 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H
# include "map.h"

t_map		*init_env(char *envp[]);
char		*xgetenv(t_map *envmap, const char *name);
char		**get_environ(t_map *map);

#endif