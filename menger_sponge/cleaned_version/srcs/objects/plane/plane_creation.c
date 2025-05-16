#include "miniRT.h"

t_object	*create_plane(t_vec3 point, t_vec3 normal, t_color color)
{
	t_object	*object;
	t_plane		*plane;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	plane = malloc(sizeof(t_plane));
	if (!plane)
	{
		free(object);
		return (NULL);
	}
	plane->normal = vec3_normalize(normal);
	plane->point = point;
	object->data = plane;
	object->material = create_material(color);
	object->type = PLANE;
	object->next = NULL;
	return (object);
}