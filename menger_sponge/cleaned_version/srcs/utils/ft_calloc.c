/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:52:52 by abillote          #+#    #+#             */
/*   Updated: 2025/05/13 10:53:13 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arrptr;

	arrptr = malloc(nmemb * size);
	if (!arrptr)
		return (NULL);
	ft_bzero(arrptr, nmemb * size);
	return (arrptr);
}
