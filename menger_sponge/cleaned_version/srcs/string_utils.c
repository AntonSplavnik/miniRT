/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:18:59 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/26 08:29:13 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	ft_strncmp(char *s1, char *s2, int n)
{
	if (NULL == s1 || NULL == s2 || n <= 0)
		return (0);
	while (*s1 == *s2 && n > 0 && *s1 != '\0')
	{
		++s1;
		++s2;
		--n;
	}
	return (*s1 - *s2);
}

void	write_string_to_file_descriptor(char *str, int file_descriptor)
{
	ssize_t	bytes_written;

	if (str == NULL || file_descriptor < 0)
		return ;
	while (*str)
	{
		bytes_written = write(file_descriptor, str, 1);
		if (bytes_written != 1)
			return ;
		str++;
	}
}
