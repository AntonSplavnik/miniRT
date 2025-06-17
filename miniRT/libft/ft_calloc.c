/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:30:37 by abillote          #+#    #+#             */
/*   Updated: 2023/11/16 12:06:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arrptr;

	arrptr = malloc(nmemb * size);
	if (!arrptr)
		return (NULL);
	ft_bzero(arrptr, nmemb * size);
	return (arrptr);
}
