/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:20:25 by asplavni          #+#    #+#             */
/*   Updated: 2025/05/13 11:09:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int ft_strcmp(char *s1, char *s2)
{
	while (*s1 == *s2 && *s1 != '\0')
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	if (NULL == s1 || NULL == s2 || n <= 0)
		return (1);
	while (n > 0 && *s1 && *s2 && *s1 == *s2)
	{
		++s1;
		++s2;
		--n;
	}
	if (n == 0 || (*s1 == *s2))
		return (0);
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
