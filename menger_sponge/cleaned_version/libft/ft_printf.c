/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 15:24:35 by abillote          #+#    #+#             */
/*   Updated: 2023/12/21 15:47:08 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printformat(const char c, va_list args)
{
	int	nb_char;

	nb_char = 0;
	if (c == '%')
		nb_char += ft_printpercent();
	else if (c == 'c')
		nb_char += ft_printchar(va_arg(args, int));
	else if (c == 's')
		nb_char += ft_printstr(va_arg(args, char *));
	else if (c == 'p')
		nb_char += ft_printptr(va_arg(args, void *));
	else if (c == 'd' || c == 'i')
		nb_char += ft_printnbr(va_arg(args, int));
	else if (c == 'u')
		nb_char += ft_printunsigned(va_arg(args, unsigned int));
	else if (c == 'x' || c == 'X')
		nb_char += ft_printhexa(va_arg(args, unsigned int), c);
	return (nb_char);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		nb_char;
	int		i;

	nb_char = 0;
	i = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			nb_char += ft_printformat(str[i], args);
		}
		else
			nb_char += ft_printchar(str[i]);
		i++;
	}
	return (nb_char);
	va_end(args);
}
