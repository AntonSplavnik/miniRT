#include "miniRT.h"

// Helper function to print status messages
void display_status(t_scene *scene)
{
	char status[100];

	if (scene->name)
	{
		// Simple sphere status
		snprintf(status, 100, "%s | Camera: (%.1f, %.1f, %.1f)", scene->name,
				scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
	}

	// Clear the window and display the status with a new image
	if (scene->mlx_connection && scene->mlx_window)
	{
		// Create a status bar at the top using a separate string display
		// Draw a black rectangle at coordinates (0,0) with width WIDTH and height 30
		mlx_string_put(scene->mlx_connection, scene->mlx_window,
					10, 20, 0xFFFF00, status);
	}
}
