/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:30:53 by abillote          #+#    #+#             */
/*   Updated: 2023/12/21 15:48:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printnbr(int c)
{
	int		nb_char;
	char	*itoa_string;

	nb_char = 0;
	ft_putnbr_fd(c, 1);
	itoa_string = ft_itoa(c);
	nb_char = ft_strlen(itoa_string);
	free(itoa_string);
	return (nb_char);
}
