/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printunsigned.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:42:20 by abillote          #+#    #+#             */
/*   Updated: 2025/06/10 12:48:36 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count_digit_unsigned(unsigned int nb)
{
	int	count;

	count = 0;
	while (nb > 9)
	{
		nb /= 10;
		count += 1;
	}
	count += 1;
	return (count);
}

void	ft_putunsigned(unsigned int nb)
{
	if (nb < 10)
	{
		nb += '0';
		write(1, &nb, 1);
	}
	else if (nb > 0)
	{
		ft_putunsigned(nb / 10);
		ft_putunsigned(nb % 10);
	}
}

int	ft_printunsigned(unsigned int nb)
{
	int	nb_char;

	nb_char = 0;
	ft_putunsigned(nb);
	nb_char = ft_count_digit_unsigned(nb);
	return (nb_char);
}
