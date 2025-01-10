/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 22:06:45 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/10 23:00:15 by tamatsuu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_status
{
	char	buffer[BUFFER_SIZE];
	ssize_t	current;
	int		fd;
	ssize_t	read_ret;
	ssize_t	bf_size;
}	t_status;

char	*get_next_line(int fd);
char	*read_file(t_status *status);
char	*output_line(ssize_t i, t_status status, char *tmp);

char	*ft_strndup(char *s, size_t len);
#endif
