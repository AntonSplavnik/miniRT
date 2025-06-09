/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:30:19 by abillote          #+#    #+#             */
/*   Updated: 2025/06/06 16:30:59 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	count_obj_elements(int fd, int *vertex_count, int *face_count)
{
    char	*line;

    *vertex_count = 0;
    *face_count = 0;
    line = get_next_line(fd);
    while (line)
    {
        if (line[0] == 'v' && line[1] == ' ')
            (*vertex_count)++;
        if (line[0] == 'f' && line[1] == ' ')
            (*face_count)++;
        free(line);
        line = get_next_line(fd);
    }
}

static void	parse_vertex_line(char *line, t_vec3 *vertex)
{
    char	**parts;

    parts = ft_split(line, ' ');
    if (!parts || !parts[1] || !parts[2] || !parts[3])
    {
        free_split(parts);
        return ;
    }
    vertex->x = ft_atof(parts[1]);
    vertex->y = ft_atof(parts[2]);
    vertex->z = ft_atof(parts[3]);
    free_split(parts);
}
