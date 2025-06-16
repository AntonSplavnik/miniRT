/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:34:59 by abillote          #+#    #+#             */
/*   Updated: 2023/11/21 11:19:36 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_free_filled(char **substr, size_t j)
{
	while (j)
	{
		free(substr[j]);
		j--;
	}
	free(substr);
	return (NULL);
}

static size_t	ft_str_count(char const *s, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (count);
}

static int	ft_substrlen(char const *s, char c, size_t start)
{
	int	len;

	len = 0;
	while (s[start] != '\0' && s[start] != c)
	{
		len++;
		start++;
	}
	return (len);
}

static char	*ft_store_substring(char const *s, char c, size_t start)
{
	int		i;
	size_t	len;
	size_t	end;
	char	*substr;

	i = 0;
	len = ft_substrlen(s, c, start);
	end = start + len;
	substr = ft_calloc(len + 1, sizeof(char));
	if (!substr)
		return (NULL);
	while (start < end)
	{
		substr[i] = s[start];
		i++;
		start++;
	}
	return (substr);
}

char	**ft_split(char const *s, char c)
{
	char	**substr;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	substr = ft_calloc(ft_str_count(s, c) + 1, sizeof(char *));
	if (!substr)
		return (NULL);
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			substr[j] = ft_store_substring(s, c, i);
			if (!substr[j])
				return (ft_free_filled(substr, j));
			j++;
		}
		while (s[i] && s[i] != c)
			i++;
	}
	return (substr);
}
