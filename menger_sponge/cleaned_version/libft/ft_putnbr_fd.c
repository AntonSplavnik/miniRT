/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 16:59:00 by abillote          #+#    #+#             */
/*   Updated: 2023/11/18 17:08:57 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//tranform single digit into characters
static void	ft_printnbr_fd(int nb, int fd)
{
	int	c;

	if (nb < 0)
	{
		write(fd, "-", 1);
		nb = -nb;
	}
	c = nb + '0';
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int nb, int fd)
{
	if (nb == -2147483648)
	{
		write(fd, "-", 1);
		write(fd, "2", 1);
		ft_putnbr_fd(147483648, fd);
	}
	else if (nb > -10 && nb < 10)
	{
		ft_printnbr_fd(nb, fd);
	}
	else if (nb > 0)
	{
		ft_putnbr_fd(nb / 10, fd);
		ft_putnbr_fd(nb % 10, fd);
	}
	else
	{
		write(fd, "-", 1);
		ft_putnbr_fd(-nb, fd);
	}
}
