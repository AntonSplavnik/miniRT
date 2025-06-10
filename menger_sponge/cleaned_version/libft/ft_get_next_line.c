/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 15:50:11 by abillote          #+#    #+#             */
/*   Updated: 2025/06/10 12:48:43 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*clean_buffer(char *stat_buffer)
{
	char			*cleaned_buffer;
	unsigned int	i;
	unsigned int	start;

	i = 0;
	start = 0;
	while (stat_buffer[start] != '\n' && stat_buffer[start])
		start++;
	if (!stat_buffer[start])
	{
		free(stat_buffer);
		return (NULL);
	}
	cleaned_buffer = ft_calloc(ft_strlen(stat_buffer) - start + 1, 1);
	start += 1;
	while (stat_buffer[start])
	{
		cleaned_buffer[i] = stat_buffer[start];
		i++;
		start++;
	}
	free(stat_buffer);
	return (cleaned_buffer);
}

char	*get_line(char *stat_buffer)
{
	int		i;
	char	*new_line;

	i = 0;
	if (!stat_buffer[i])
		return (NULL);
	while (stat_buffer[i] != '\n' && stat_buffer[i])
		i++;
	new_line = ft_calloc(i + 2, sizeof(char));
	if (!new_line)
		return (NULL);
	i = 0;
	while (stat_buffer[i] != '\n' && stat_buffer[i])
	{
		new_line[i] = stat_buffer[i];
		i++;
	}
	if (stat_buffer[i] == '\n' && stat_buffer[i])
		new_line[i++] = '\n';
	new_line[i] = '\0';
	return (new_line);
}

char	*buffer_join(char *stat_buffer, char *temp_buffer)
{
	char	*joined_buffer;

	joined_buffer = ft_strjoin(stat_buffer, temp_buffer);
	free(stat_buffer);
	return (joined_buffer);
}

char	*store_file(int fd, char *stat_buffer)
{
	char	*temp_buffer;
	int		size_read;

	if (!stat_buffer)
		stat_buffer = ft_calloc(1, 1);
	if (!stat_buffer)
		return (NULL);
	temp_buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	size_read = 1;
	while (size_read > 0)
	{
		size_read = read(fd, temp_buffer, BUFFER_SIZE);
		if (size_read == -1)
		{
			free(temp_buffer);
			return (NULL);
		}
		temp_buffer[size_read] = 0;
		stat_buffer = buffer_join(stat_buffer, temp_buffer);
		if (ft_strchr(temp_buffer, '\n'))
			break ;
	}
	free (temp_buffer);
	return (stat_buffer);
}

char	*get_next_line(int fd)
{
	char		*new_line;
	static char	*stat_buffer[1024];

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	stat_buffer[fd] = store_file(fd, stat_buffer[fd]);
	if (!stat_buffer[fd])
		return (NULL);
	new_line = get_line(stat_buffer[fd]);
	stat_buffer[fd] = clean_buffer(stat_buffer[fd]);
	return (new_line);
}
