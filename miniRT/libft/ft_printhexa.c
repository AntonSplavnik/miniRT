/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhexa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:58:47 by abillote          #+#    #+#             */
/*   Updated: 2023/12/21 15:48:47 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_hexa_len(unsigned int nb)
{
	int	nb_char;

	nb_char = 0;
	while (nb > 0)
	{
		nb /= 16;
		nb_char++;
	}
	return (nb_char);
}

void	ft_puthexa(unsigned int nb, const char c)
{
	if (nb >= 16)
	{
		ft_puthexa(nb / 16, c);
		ft_puthexa(nb % 16, c);
	}
	else
	{
		if (nb <= 9)
			ft_printchar((nb + '0'));
		else if (c == 'x')
			ft_printchar((nb - 10 + 'a'));
		else if (c == 'X')
			ft_printchar((nb - 10 + 'A'));
	}
}

int	ft_printhexa(unsigned int nb, const char c)
{
	int	nb_char;

	nb_char = 0;
	if (nb == 0)
	{
		write(1, "0", 1);
		nb_char++;
	}
	else
	{
		ft_puthexa(nb, c);
		nb_char += ft_hexa_len(nb);
	}
	return (nb_char);
}
