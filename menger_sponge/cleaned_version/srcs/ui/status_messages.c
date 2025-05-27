#include "../../includes/miniRT.h"

// Helper function to print status messages
void display_status(t_scene *scene)
{
	char status[100];
	static mlx_image_t *status_img = NULL;

	if (!scene || !scene->app.mlx)
		return;

	// Delete previous status text image if it exists
	if (status_img != NULL)
	{
		mlx_delete_image(scene->app.mlx, status_img);
		status_img = NULL;
	}

	if (scene->name)
	{
		// Simple sphere status
		snprintf(status, 100, "%s | Camera: (%.1f, %.1f, %.1f)", scene->name,
				scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
	}

	// Display the status with a new image
	if (scene->app.mlx)
	{
		// Create a new text image with the status message
		status_img = mlx_put_string(scene->app.mlx, status, 10, 20);
	}
}
