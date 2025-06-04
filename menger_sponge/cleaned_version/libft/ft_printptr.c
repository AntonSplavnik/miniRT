/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 11:35:20 by abillote          #+#    #+#             */
/*   Updated: 2023/12/21 15:48:34 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_ptr_len(uintptr_t address)
{
	int	nb_char;

	nb_char = 0;
	while (address > 0)
	{
		address /= 16;
		nb_char++;
	}
	return (nb_char);
}

void	ft_putptr(uintptr_t address)
{
	if (address >= 16)
	{
		ft_putptr(address / 16);
		ft_putptr(address % 16);
	}
	else
	{
		if (address <= 9)
			ft_printchar((address + '0'));
		else
			ft_printchar((address - 10 + 'a'));
	}
}

int	ft_printptr(void *ptr)
{
	int			nb_char;
	uintptr_t	address;

	nb_char = 0;
	address = (uintptr_t)ptr;
	if (address == 0)
	{
		write(1, "(nil)", 5);
		nb_char += 5;
	}
	else
	{
		write(1, "0x", 2);
		nb_char = 2;
		ft_putptr(address);
		nb_char += ft_ptr_len(address);
	}
	return (nb_char);
}
