/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 15:47:21 by abillote          #+#    #+#             */
/*   Updated: 2023/11/22 17:16:53 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static void	ft_count_digit(int n, size_t *count, size_t *weight)
{
	*count = 0;
	*weight = 1;
	if (n < 0)
	{
		*count += 1;
		if (n == -2147483648)
		{
			*count += 1;
			n = -147483648;
		}
		n = -n;
	}
	while (n > 9)
	{
		n /= 10;
		*count += 1;
		*weight *= 10;
	}
	*count += 1;
}

static char	*ft_fill_string(int n, size_t weight, char *str)
{
	size_t	i;

	i = 0;
	if (n < 0)
	{
		str[i++] = '-';
		if (n == -2147483648)
		{
			str[i++] = 2 + '0';
			n = -147483648;
		}
		n = -n;
	}
	while (n >= 0 && weight > 0)
	{
		str[i++] = (n / weight) + '0';
		n %= weight;
		weight /= 10;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	count;
	size_t	weight;

	ft_count_digit(n, &count, &weight);
	str = (char *)malloc((count + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str = ft_fill_string(n, weight, str);
	return (str);
}
