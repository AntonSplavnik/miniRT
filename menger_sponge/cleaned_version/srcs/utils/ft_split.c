/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:34:59 by abillote          #+#    #+#             */
/*   Updated: 2025/05/13 13:38:27 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

char	**ft_split(char *str, char c)
{
	char	**result;
	int		word_count;
	int		i;
	int		j;
	int		start;

	if (!str)
		return (NULL);
	word_count = 0;
	i = 0;
	// Count words
	while (str[i])
	{
		while (str[i] && (str[i] == c || is_whitespace(str[i])))
			i++;
		if (str[i])
			word_count++;
		while (str[i] && str[i] != c && !is_whitespace(str[i]))
			i++;
	}
	// Allocate array
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	// Fill array
	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == c || is_whitespace(str[i])))
			i++;
		if (!str[i])
			break;
		start = i;
		while (str[i] && str[i] != c && !is_whitespace(str[i]))
			i++;
		result[j] = (char *)malloc(sizeof(char) * (i - start + 1));
		if (!result[j])
		{
			while (j > 0)
				free(result[--j]);
			free(result);
			return (NULL);
		}
		ft_strlcpy(result[j], &str[start], i - start + 1);
		j++;
	}
	result[j] = NULL;
	return (result);
}
