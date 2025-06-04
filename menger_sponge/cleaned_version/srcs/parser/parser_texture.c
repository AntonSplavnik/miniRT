/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:36:27 by abillote          #+#    #+#             */
/*   Updated: 2025/06/04 16:38:26 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_texture *create_texture(t_scene *scene, const char *filename)
{
	t_texture *texture;
	mlx_texture_t *mlx_texture;

	texture = malloc(sizeof(t_texture));
	if (!texture)
		return NULL;
	texture->filename = ft_strdup(filename);
	if (!texture->filename)
	{
		free(texture);
		return NULL;
	}
	mlx_texture = mlx_load_png(texture->filename);
	if (!mlx_texture)
	{
		free(texture->filename);
		free(texture);
		parse_error(scene, "Failed to load texture from file");
	}
	texture->width = mlx_texture->width;
	texture->height = mlx_texture->height;
	texture->mlx_texture = mlx_texture;
	texture->data = malloc(mlx_texture->width * mlx_texture->height * 4); // Assuming RGBA format
	if (!texture->data)
	{
		printf("Error: Could not allocate memory for texture data\n");
		free_texture_mlx(texture);
		return NULL;
	}
	memcpy(texture->data, mlx_texture->pixels, mlx_texture->width * mlx_texture->height * 4);
	return texture;
}

void free_texture_mlx(t_texture *texture)
{
	if (!texture)
		return;

	// Free the filename if it exists
	if (texture->filename)
	{
		free(texture->filename);
		texture->filename = NULL;
	}

	// Free the MLX texture if it exists
	if (texture->mlx_texture)
	{
		mlx_delete_texture(texture->mlx_texture);
		texture->mlx_texture = NULL;
	}

	// Free the data if it exists
	if (texture->data)
	{
		free(texture->data);
		texture->data = NULL;
	}

	free(texture);
}
