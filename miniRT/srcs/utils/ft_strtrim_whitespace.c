/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_whitespace.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:22:48 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 10:51:03 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Helper function to find the start index for trimming
static int	find_trim_start(char *s)
{
	int	i;

	i = 0;
	while (s[i] && ft_isspace(s[i]))
		i++;
	return (i);
}

// Helper function to find the end index for trimming
static int	find_trim_end(char *s, int start)
{
	int	end;

	end = start;
	while (s[end])
		end++;
	if (end > 0)
		end--;
	while (end >= start && ft_isspace(s[end]))
		end--;
	return (end);
}

// Trim whitespace from the start and end of a string
char	*ft_strtrim_whitespace(char *s)
{
	int		start;
	int		end;
	char	*result;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	start = find_trim_start(s);
	end = find_trim_end(s, start);
	result = (char *)malloc(end - start + 2);
	if (!result)
		return (NULL);
	i = start;
	j = 0;
	while (i <= end)
	{
		result[j] = s[i];
		i++;
		j++;
	}
	result[j] = '\0';
	return (result);
}
