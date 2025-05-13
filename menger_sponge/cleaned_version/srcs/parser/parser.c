/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:08:35 by abillote          #+#    #+#             */
/*   Updated: 2025/05/13 13:14:41 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"
#include <fcntl.h>


void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void	parse_error(t_scene *scene, char *message)
{
	cleanup_scene(scene);
	write_string_to_file_descriptor("Error\n", STDERR_FILENO);
	write_string_to_file_descriptor(message, STDERR_FILENO);
	write_string_to_file_descriptor("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

int	is_ambient(char *line)
{
	if (line[0] == 'A')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_camera(char *line)
{
	if (line[0] == 'C')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_light(char *line)
{
	if (line[0] == 'L')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_sphere(char *line)
{
	if (ft_strncmp(line, "sp", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_plane(char *line)
{
	if (ft_strncmp(line, "pl", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_cylinder(char *line)
{
	if (ft_strncmp(line, "cy", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_cone(char *line)
{
	if (ft_strncmp(line, "co", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_triangle(char *line)
{
	if (ft_strncmp(line, "tr", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_cube(char *line)
{
	if (ft_strncmp(line, "cu", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	check_ambient_uniqueness(t_scene *scene)
{
	if (scene->ambient.ratio != 0.1)
		return (0);
	if (scene->ambient.color.r != 255 || scene->ambient.color.g != 255 || scene->ambient.color.b != 255)
		return (0);
	return (1);
}

int	check_range(double value, double min, double max)
{
	if (value < min || value > max)
		return (0);
	return (1);
}

int	read_ambient_ratio(t_scene *scene, char **str, double *ratio)
{
	if (!str)
		return (0);
	*ratio = ft_atof(str[1]);
	if (!check_range(*ratio, 0.0, 1.0))
	{
		free_split(str);
		parse_error(scene, "Ambient ratio must be between 0.0 and 1.0");
		return (0);
	}
	return (1);
}

int	parse_rgb(char **parts, int *r, int *g, int *b)
{
	*r = ft_atoi(parts[0]);
	*g = ft_atoi(parts[1]);
	*b = ft_atoi(parts[2]);
	if (!check_range(*r, 0, 255) || !check_range(*g, 0, 255) || !check_range(*b, 0, 255))
		return (0);
	return (1);
}

int	read_color(char *str, t_color *color)
{
	char	**parts;
	int		parts_count;
	int		r;
	int		b;
	int		g;
	int		parse_result;

	parts = ft_split(str, ',');
	if (!parts)
		return (0);
	parts_count = 0;
	while (parts[parts_count])
		parts_count++;
	if (parts_count != 3)
	{
		free_split(parts);
		return (0);
	}
	parse_result = parse_rgb(parts, &r, &g, &b);
	free_split(parts);
	if (!parse_result)
		return (0);
	*color = create_color(r, g, b);
	return (1);
}

int	parse_ambient(t_scene *scene, char *line)
{
	char	**parts;
	int		parts_count;
	int		parse_color;

	if (!check_ambient_uniqueness(scene))
		parse_error(scene, "Ambient lighting can only be declared once");
	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	parts_count = 0;
	while (parts[parts_count])
		parts_count++;
	if (parts_count != 3)
	{
		free_split(parts);
		parse_error(scene, "Invalid format for ambient lighting. Expected: A ratio r,g,b");
	}
	read_ambient_ratio(scene, parts, &scene->ambient.ratio);
	parse_color = read_color(parts[2], &scene->ambient.color);
	free_split(parts);
	if (!parse_color)
		parse_error(scene, "Invalid color format for ambient lighting. Expected: r,g,b");
	return (1);
}

int	count_parts(char **parts)
{
	int	count;

	count = 0;
	while (parts[count])
		count++;
	return (count);
}

void	check_parts_count(t_scene *scene, char **parts, int expected_count, char *element_name)
{
	int	parts_count;

	parts_count = count_parts(parts);
	if (parts_count != expected_count)
	{
		free_split(parts);
		if (ft_strcmp(element_name, "camera") == 0)
			parse_error(scene, "Invalid format for camera. Expected: C o,d,f");
		else if (ft_strcmp(element_name, "ambient") == 0)
			parse_error(scene, "Invalid format for ambient lighting. Expected: A ratio r,g,b");
		else if (ft_strcmp(element_name, "light") == 0)
			parse_error(scene, "Invalid format for light. Expected: L ratio r,g,b");
		else if (ft_strcmp(element_name, "sphere") == 0)
			parse_error(scene, "Invalid format for sphere. Expected: sp x,y,z r");
		else if (ft_strcmp(element_name, "plane") == 0)
			parse_error(scene, "Invalid format for plane. Expected: pl x,y,z nx,ny,nz");
		else if (ft_strcmp(element_name, "cylinder") == 0)
			parse_error(scene, "Invalid format for cylinder. Expected: cy x,y,z d,h");
		else if (ft_strcmp(element_name, "cone") == 0)
			parse_error(scene, "Invalid format for cone. Expected: co x,y,z nx,ny,nz radius height r,g,b");
		else if (ft_strcmp(element_name, "triangle") == 0)
			parse_error(scene, "Invalid format for triangle. Expected: tr x1,y1,z1 x2,y2,z2 x3,y3,z3 r,g,b");
		else if (ft_strcmp(element_name, "cube") == 0)
			parse_error(scene, "Invalid format for cube. Expected: cu x,y,z side_length");
	}
}

int check_camera_uniqueness(t_scene *scene)
{
	if (scene->camera.position.x != 0.0 || scene->camera.position.y != 0.0 || scene->camera.position.z != 0.0)
		return (0);
	return (1);
}

int	check_vector_normalization(t_vec3 vector)
{
	if (vector.x < -1.0 || vector.x > 1.0 || vector.y < -1.0 || vector.y > 1.0 || vector.z < -1.0 || vector.z > 1.0)
		return (0);
	return (1);
}

int	read_vector(char *str, t_vec3 *vector)
{
	char	**parts;
	double	x;
	double	y;
	double	z;
	int		parts_count;

	parts = ft_split(str, ',');
	if (!parts)
		return (0);
	parts_count = count_parts(parts);
	if (parts_count != 3)
	{
		free_split(parts);
		return (0);
	}
	x = ft_atof(parts[0]);
	y = ft_atof(parts[1]);
	z = ft_atof(parts[2]);
	free_split(parts);
	*vector = vec3_create(x, y, z);
	return (1);
}

int	parse_camera(t_scene *scene, char *line)
{
	char	**parts;
	int		parse_result;

	if (!check_camera_uniqueness(scene))
		parse_error(scene, "Camera can only be declared once");
	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "camera");
	if (!read_vector(parts[1], &scene->camera.position))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for camera position. Expected: o,d,f");
	}
	parse_result = read_vector(parts[2], &scene->camera.rotation);
	if (!parse_result || !check_vector_normalization(scene->camera.rotation))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for camera rotation. Expected: r,p,y");
	}
	scene->camera.fov = ft_atof(parts[3]);
	free_split(parts);
	if (!check_range(scene->camera.fov, 0.0, 180.0))
		parse_error(scene, "Camera FOV must be between 0.0 and 180.0");
	return (1);
}

int parse_light(t_scene *scene, char *line)
{
	char	**parts;
	int		parse_result;
	t_light *new_light;
	t_vec3  position;
	double  intensity;
	t_color color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "light");
	if (!read_vector(parts[1], &position))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for light position. Expected: x,y,z");
	}
	intensity = ft_atof(parts[2]);
	if (!check_range(intensity, 0.0, 1.0))
	{
		free_split(parts);
		parse_error(scene, "Light ratio must be between 0.0 and 1.0");
	}
	parse_result = read_color(parts[3], &color);
	free_split(parts);
	if (!parse_result)
		parse_error(scene, "Invalid format for light color. Expected: r,g,b");

	new_light = create_light(position, intensity, color);
	if (!new_light)
		parse_error(scene, "Failed to create light");
	add_light(scene, new_light);
	return (1);
}

int	parse_sphere_data(t_scene *scene, char **parts, t_vec3 *center, double *diameter, t_color *color)
{
	if (!read_vector(parts[1], center))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for sphere position. Expected: x,y,z");
	}
	*diameter = ft_atof(parts[2]);
	if (!check_range(*diameter, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Sphere diameter must be a positive number");
	}
	if (!read_color(parts[3], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for sphere color. Expected: r,g,b");
	}
	return (1);
}
int	parse_sphere(t_scene *scene, char *line)
{
	char		**parts;
	int			parse_result;
	t_object	*sphere;
	t_vec3		center;
	double		diameter;
	t_color		color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "sphere");
	parse_result = parse_sphere_data(scene, parts, &center, &diameter, &color);
	if (!parse_result)
		return (0);
	free_split(parts);
	sphere = create_sphere(center, diameter, color);
	if(!sphere)
		parse_error(scene, "Failed to create sphere");
	add_object(scene, sphere);
	return (1);
}

void	parse_plane_data(t_scene *scene, char **parts, t_vec3 *point, t_vec3 *normal, t_color *color)
{
	if (!read_vector(parts[1], point))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane position. Expected: x,y,z");
	}
	if (!read_vector(parts[2], normal))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane normal. Expected: nx,ny,nz");
	}
	if (!check_vector_normalization(*normal))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane normal. Expected: nx,ny,nz");
	}
	if (!read_color(parts[3], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane color. Expected: r,g,b");
	}
}
int	parse_plane(t_scene *scene, char *line)
{
	char	**parts;
	t_object	*plane;
	t_vec3		point;
	t_vec3		normal;
	t_color		color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "plane");
	parse_plane_data(scene, parts, &point, &normal, &color);
	free_split(parts);
	plane = create_plane(point, normal, color);
	if(!plane)
		parse_error(scene, "Failed to create plane");
	add_object(scene, plane);
	return (1);
}

void	parse_cylinder_vectors(t_scene *scene, char **parts, t_vec3 *center, t_vec3 *axis)
{
	if (!read_vector(parts[1], center))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder center Expected: x,y,z");
	}
	if (!read_vector(parts[2], axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder axis. Expected: nx,ny,nz");
	}
	if (!check_vector_normalization(*axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder axis. Expected: nx,ny,nz");
	}
}
void	parse_cylinder_data(t_scene *scene, char **parts, double *diameter, double *height, t_color *color)
{
	*diameter = ft_atof(parts[3]);
	*height = ft_atof(parts[4]);
	if (!check_range(*diameter, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder diameter. Expected: positive number");
	}
	if (!check_range(*height, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder height. Expected: positive number");
	}
	if (!read_color(parts[5], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder color. Expected: r,g,b");
	}
}

int	parse_cylinder(t_scene *scene, char *line)
{
	char	**parts;
	t_object	*cylinder;
	t_vec3		center;
	t_vec3		axis;
	double		diameter;
	double		height;
	t_color		color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 6, "cylinder");
	parse_cylinder_vectors(scene, parts, &center, &axis);
	parse_cylinder_data(scene, parts, &diameter, &height, &color);
	free_split(parts);
	cylinder = create_cylinder(center, axis, diameter, height);
	if(!cylinder)
		parse_error(scene, "Failed to create cylinder");
	cylinder->material.color = color;
	add_object(scene, cylinder);
	return (1);
}

void	parse_cone_vectors(t_scene *scene, char **parts, t_vec3 *tip, t_vec3 *axis)
{
	if (!read_vector(parts[1], tip))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone tip. Expected: x,y,z");
	}
	if (!read_vector(parts[2], axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone axis. Expected: nx,ny,nz");
	}
	if (!check_vector_normalization(*axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone axis. Expected: nx,ny,nz");
	}
}
void	parse_cone_data(t_scene *scene, char **parts, double *radius, double *height, t_color *color)
{
	*radius = ft_atof(parts[3]);
	*height = ft_atof(parts[4]);
	if (!check_range(*radius, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone radius. Expected: positive number");
	}
	if (!check_range(*height, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone height. Expected: positive number");
	}
	if (!read_color(parts[5], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone color. Expected: r,g,b");
	}
}

// Input format:co x,y,z nx,ny,nz radius height r,g,b
int	parse_cone(t_scene *scene, char *line)
{
	char	**parts;
	t_object	*cone;
	t_vec3		tip;
	t_vec3		axis;
	double		radius;
	double		height;
	t_color		color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 6, "cone");
	parse_cone_vectors(scene, parts, &tip, &axis);
	parse_cone_data(scene, parts, &radius, &height, &color);
	free_split(parts);
	cone = create_cone(tip, axis, radius, height);
	if(!cone)
		parse_error(scene, "Failed to create cone");
	cone->material.color = color;
	add_object(scene, cone);
	return (1);
}

//void	parse_triangle_data(t_scene *scene, char **parts, t_vec3 *v0, t_vec3 *v1, t_vec3 *v2)
//{
//	if (!read_vector(parts[1], v0))
//	{
//		free_split(parts);
//		parse_error(scene, "Invalid format for triangle vertex 1. Expected: x,y,z");
//	}
//	if (!read_vector(parts[2], v1))
//	{
//		free_split(parts);
//		parse_error(scene, "Invalid format for triangle vertex 2. Expected: x,y,z");
//	}
//	if (!read_vector(parts[3], v2))
//	{
//		free_split(parts);
//		parse_error(scene, "Invalid format for triangle vertex 3. Expected: x,y,z");
//	}
//}

//int	parse_triangle(t_scene *scene, char *line)
//{
//	char	**parts;
//	t_object	*triangle;
//	t_vec3		v0;
//	t_vec3		v1;
//	t_vec3		v2;
//	t_color		color;

//	parts = ft_split(line, ' ');
//	if (!parts)
//		parse_error(scene, "Failed to split line");
//	check_parts_count(scene, parts, 5, "triangle");
//	parse_triangle_data(scene, parts, &v0, &v1, &v2);
//	if (!read_color(parts[4], &color))
//	{
//		free_split(parts);
//		parse_error(scene, "Invalid format for triangle color. Expected: r,g,b");
//	}
//	free_split(parts);
//	triangle = create_triangle(v0, v1, v2, color);
//	if (!triangle)
//		parse_error(scene, "Failed to create triangle");
//	add_object(scene, triangle);
//	return (1);
//}

//void	parse_cube_data(t_scene *scene, char **parts, t_vec3 *center, double *side_length, t_color *color)
//{
//	if (!read_vector(parts[1], center))
//	{
//		free_split(parts);
//		parse_error(scene, "Invalid format for cube center. Expected: x,y,z");
//	}
//	*side_length = ft_atof(parts[2]);
//	if (!check_range(*side_length, 0.0, INFINITY))
//	{
//		free_split(parts);
//		parse_error(scene, "Invalid format for cube side length. Expected: positive number");
//	}
//	if (!read_color(parts[3], color))
//	{
//		free_split(parts);
//		parse_error(scene, "Invalid format for cube color. Expected: r,g,b");
//	}
//}

//int	parse_cube(t_scene *scene, char *line)
//{
//	char	**parts;
//	t_object	*cube;
//	t_vec3		center;
//	double		side_length;
//	t_color		color;

//	parts = ft_split(line, ' ');
//	if (!parts)
//		parse_error(scene, "Failed to split line");
//	check_parts_count(scene, parts, 5, "cube");
//	parse_cube_data(scene, parts, &center, &side_length, &color);
//	free_split(parts);
//	cube = create_cube(center, side_length, color);
//	if (!cube)
//		parse_error(scene, "Failed to create cube");
//	add_object(scene, cube);
//	return (1);
//}

int	parse_parameters(t_scene *scene, char *line)
{
	if (is_ambient(line))
		return (parse_ambient(scene, line));
	else if (is_camera(line))
		return (parse_camera(scene, line));
	else if (is_light(line))
		return (parse_light(scene, line));
	else if (is_sphere(line))
		return (parse_sphere(scene, line));
	else if (is_plane(line))
		return (parse_plane(scene, line));
	else if (is_cylinder(line))
		return (parse_cylinder(scene, line));
	else if (is_cone(line))
		return (parse_cone(scene, line));
	//else if (is_triangle(line))
	//	return (parse_triangle(scene, line));
	//else if (is_cube(line))
	//	return (parse_cube(scene, line));
	else
		return (0);
}

int	parse_line(t_scene *scene, char *line)
{
	char	*trimmed;
	int		result;

	trimmed = ft_strtrim(line);
	if (!trimmed)
		return (0);
	if (trimmed[0] == '\0' || trimmed[0] == '#')
	{
		free(trimmed);
		return (1);
	}
	result = parse_parameters(scene, trimmed);
	if (result == 0)
	{
		free(trimmed);
		parse_error(scene, "Unknown element identifier");
		return (0);
	}
	free(trimmed);
	return (result);
}

//Find the end of the current line in the buffer if it exists otherwise return the end of the buffer
static int	process_buffer_segment(char *buffer, int *i, int bytes_read)
{
	int	line_end;

	line_end = *i;
	while (line_end < bytes_read && buffer[line_end] != '\n')
		line_end++;
	return (line_end);
}

//Copy the current line from the buffer to the line array
static int	copy_to_line(t_scene *scene, char *buffer, int *pos, char *line)
{
	int	start;
	int	end;

	start = pos[0];
	end = pos[1];
	if (pos[2] + (end - start) >= MAX_LINE_LEN)
		parse_error(scene, "Line too long in scene file");
	while (start < end)
	{
		line[pos[2]] = buffer[start];
		pos[2]++;
		start++;
	}
	line[pos[2]] = '\0';
	return (1);
}

//Using an array of positions to reduce the number of parameters
//pos[0] is the current position in the buffer
//pos[1] is the end of the current line
//pos[2] is the current position in the line
int	read_scene_file(int fd, t_scene *scene)
{
	int			bytes_read;
	static char	line[MAX_LINE_LEN + 1];
	char		buffer[MAX_LINE_LEN + 1];
	int			pos[3];

	pos[2] = 0;
	while ((bytes_read = read(fd, buffer, MAX_LINE_LEN)) > 0)
	{
		buffer[bytes_read] = '\0';
		pos[0] = 0;
		while (pos[0] < bytes_read)
		{
			pos[1] = process_buffer_segment(buffer, &pos[0], bytes_read);
			copy_to_line(scene, buffer, pos, line);
			if (pos[1] < bytes_read && buffer[pos[1]] == '\n')
			{
				if (!parse_line(scene, line))
					return (0);
				pos[2] = 0;
			}
			pos[0] = pos[1] + 1;
		}
	}
	return (bytes_read);
}

//Check if the filename ends with ".rt"
int is_valid_filename(char *filename)
{
	if (!filename)
		return (0);
	if (ft_strlen(filename) < 3)
		return (0);
	if (ft_strcmp(filename + ft_strlen(filename) - 3, ".rt") != 0)
		return (0);
	return (1);
}

// Parse file part 1: Open file and prepare for reading
int	open_scene_file(t_scene *scene, char *filename)
{
	int	fd;

	if (!is_valid_filename(filename))
	{
		parse_error(scene, "Invalid filename");
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		parse_error(scene, "Failed to open file");
	return (fd);
}

/*
*	Parse the scene file and return a t_scene struct
*	Handle the case where the last line of the file does not end with a newline
*	Return 1 on success, 0 on failure
*/
int	parse_scene_file(char *filename, t_scene *scene)
{
	int			fd;
	int			bytes_read;
	static char	line[MAX_LINE_LEN + 1];
	int			line_pos;

	fd = open_scene_file(scene, filename);
	if (fd < 0)
		return (0);
	bytes_read = read_scene_file(fd, scene);
	line_pos = 0;
	while (line[line_pos])
		line_pos++;
	if (line_pos > 0 && !parse_line(scene, line))
	{
		close(fd);
		return (0);
	}
	if (bytes_read < 0)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}





