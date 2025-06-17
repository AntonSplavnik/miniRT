/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 15:39:35 by abillote          #+#    #+#             */
/*   Updated: 2023/11/18 16:22:45 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*snew;
	unsigned int	i;

	i = 0;
	snew = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!snew)
		return (NULL);
	while (s[i] != '\0')
	{
		snew[i] = (*f)(i, s[i]);
		i++;
	}
	snew[i] = '\0';
	return (snew);
}
