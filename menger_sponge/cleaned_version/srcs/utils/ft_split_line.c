/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:34:59 by abillote          #+#    #+#             */
/*   Updated: 2025/06/05 14:43:16 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

static int	is_separator(char c, char delimiter)
{
	return (c == delimiter || is_whitespace(c));
}

static int	count_words(char *str, char delimiter)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && is_separator(str[i], delimiter))
			i++;
		if (str[i])
			count++;
		while (str[i] && !is_separator(str[i], delimiter))
			i++;
	}
	return (count);
}

static void	free_split_array(char **array, int up_to)
{
	while (up_to > 0)
		free(array[--up_to]);
	free(array);
}

static char	*extract_word(char *str, int start, int end)
{
	char	*word;

	word = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, &str[start], end - start + 1);
	return (word);
}

static int	fill_words(char **result, char *str, char delimiter)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && is_separator(str[i], delimiter))
			i++;
		if (!str[i])
			break;
		start = i;
		while (str[i] && !is_separator(str[i], delimiter))
			i++;
		result[j] = extract_word(str, start, i);
		if (!result[j])
		{
			free_split_array(result, j);
			return (0);
		}
		j++;
	}
	result[j] = NULL;
	return (1);
}

char	**ft_split_line(char *str, char c)
{
	char	**result;
	int		word_count;

	if (!str)
		return (NULL);
	word_count = count_words(str, c);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	if (!fill_words(result, str, c))
		return (NULL);
	return (result);
}
