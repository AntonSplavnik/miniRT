#include "../../../includes/miniRT.h"

//Note: Due to args limitation (norminette), we need to modify the color later, after cylinder creation
t_object	*create_cylinder(t_vec3 center, t_vec3 axis, double diameter, double height)
{
	t_object	*object;
	t_cylinder	*cylinder;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
	{
		free(object);
		return (NULL);
	}
	cylinder->center = center;
	cylinder->axis = vec3_normalize(axis);
	cylinder->diameter = diameter;
	cylinder->radius = diameter / 2.0;
	cylinder->height = height;
	object->data = cylinder;
	object->material = create_material(create_color(255, 255, 255));
	object->type = CYLINDER;
	object->next = NULL;
	return (object);
}