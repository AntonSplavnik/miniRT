/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:18:59 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/13 19:39:36 by parallels        ###   ########.fr       */
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

/* void	write_string_to_file_descriptor(char *str, int file_descriptor)
{
	if (NULL == str || file_descriptor < 0)
		return ;
	if (*str != '\0')
	{
		write(file_descriptor, str, 1);
		write_string_to_file_descriptor(str + 1, file_descriptor);
	}
} */

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


double	string_to_double(char *str)
{
	long	integer_part;
	double	fractional_part;
	double	decimal_divisor;
	int		sign;

	integer_part = 0;
	fractional_part = 0;
	sign = 1;
	decimal_divisor = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		++str;
	while (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -sign;
	while (*str != '.' && *str)
		integer_part = (integer_part * 10) + (*str++ - '0');
	if (*str == '.')
		++str;
	while (*str)
	{
		decimal_divisor /= 10;
		fractional_part = fractional_part + (*str++ - '0') * decimal_divisor;
	}
	return ((integer_part + fractional_part) * sign);
}
