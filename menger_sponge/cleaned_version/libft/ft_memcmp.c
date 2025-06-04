/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:43:45 by abillote          #+#    #+#             */
/*   Updated: 2023/11/15 17:49:33 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*ptrs1;
	const unsigned char	*ptrs2;

	i = 0;
	ptrs1 = s1;
	ptrs2 = s2;
	if (n == 0)
		return (0);
	while (*ptrs1 == *ptrs2 && i < n - 1)
	{
		ptrs1++;
		ptrs2++;
		i++;
	}
	return (*ptrs1 - *ptrs2);
}
