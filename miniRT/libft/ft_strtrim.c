/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:22:48 by abillote          #+#    #+#             */
/*   Updated: 2023/11/23 15:47:50 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	c_to_trim(char const *set, char c)
{
	size_t	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*create_new_string(char const *s1, size_t start, size_t end)
{
	char	*strim;
	size_t	i;

	i = 0;
	if (start > end || start > ft_strlen(s1))
	{
		strim = ft_calloc(1, sizeof(char));
		if (!strim)
			return (NULL);
		strim[0] = '\0';
		return (strim);
	}
	strim = ft_calloc((end - start + 2), sizeof(char));
	if (!strim)
		return (NULL);
	while (start <= end)
		strim[i++] = s1[start++];
	strim[i] = '\0';
	return (strim);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*strim;
	size_t	start;
	size_t	end;

	start = 0;
	end = ft_strlen(s1) - 1;
	if (s1 == NULL || set == NULL)
		return (NULL);
	if (ft_strlen(s1) == 0)
	{
		strim = ft_calloc(1, sizeof(char));
		if (!strim)
			return (NULL);
		strim[0] = '\0';
		return (strim);
	}
	while (c_to_trim(set, s1[start]) != 0)
		start++;
	while (c_to_trim(set, s1[end]) != 0 && end != 0)
		end--;
	strim = create_new_string(s1, start, end);
	return (strim);
}
