/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:43:45 by abillote          #+#    #+#             */
/*   Updated: 2023/11/22 17:19:33 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*ptrsrc;
	unsigned char		*ptrdest;
	size_t				i;

	ptrsrc = src;
	ptrdest = dest;
	if (!src && !dest)
		return (NULL);
	if (ptrsrc < ptrdest && ptrdest < ptrsrc + n)
	{
		while (n-- > 0)
			ptrdest[n] = ptrsrc[n];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			ptrdest[i] = ptrsrc[i];
			i++;
		}
	}
	return (dest);
}
