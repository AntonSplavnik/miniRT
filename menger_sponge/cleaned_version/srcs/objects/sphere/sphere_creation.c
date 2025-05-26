#include "../../../includes/miniRT.h"

t_object	*create_sphere(t_vec3 center, double diameter, t_color color)
{
	t_object	*object;
	t_sphere	*sphere;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
	{
		free(object);
		return (NULL);
	}
	sphere->center = center;
	sphere->diameter = diameter;
	sphere->radius = diameter / 2.0;
	object->type = SPHERE;
	object->material = create_material(color);
	object->data = sphere;
	object->next = NULL;
	return (object);
}
