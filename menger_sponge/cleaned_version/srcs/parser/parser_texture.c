/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:36:27 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 14:23:22 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	fill_texture_mlx(t_texture *texture, mlx_texture_t *mlx_texture, \
							t_scene *scene)
{
	texture->width = mlx_texture->width;
	texture->height = mlx_texture->height;
	texture->mlx_texture = mlx_texture;
	texture->data = malloc(mlx_texture->width * mlx_texture->height * 4);
	if (!texture->data)
	{
		free_texture_mlx(texture);
		parse_error(scene, "Failed to allocate memory for texture data");
	}
	memcpy(texture->data, mlx_texture->pixels, \
				mlx_texture->width * mlx_texture->height * 4);
}

t_texture	*create_texture(t_scene *scene, const char *filename)
{
	t_texture		*texture;
	mlx_texture_t	*mlx_texture;

	texture = malloc(sizeof(t_texture));
	if (!texture)
		parse_error(scene, "Failed to allocate memory for texture");
	*texture = (t_texture){.filename = NULL, .mlx_texture = NULL, \
								.data = NULL, .width = 0, .height = 0};
	texture->filename = ft_strdup(filename);
	if (!texture->filename)
	{
		free(texture);
		parse_error(scene, "Failed to allocate memory for texture");
	}
	mlx_texture = mlx_load_png(texture->filename);
	if (!mlx_texture)
	{
		free_texture_mlx(texture);
		parse_error(scene, "Failed to load texture from file");
	}
	fill_texture_mlx(texture, mlx_texture, scene);
	return (texture);
}

void	free_texture_mlx(t_texture *texture)
{
	if (!texture)
		return ;
	if (texture->filename)
	{
		free(texture->filename);
		texture->filename = NULL;
	}
	if (texture->mlx_texture)
	{
		mlx_delete_texture(texture->mlx_texture);
		texture->mlx_texture = NULL;
	}
	if (texture->data)
	{
		free(texture->data);
		texture->data = NULL;
	}
	free(texture);
}
