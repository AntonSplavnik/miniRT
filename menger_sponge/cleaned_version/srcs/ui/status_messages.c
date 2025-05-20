#include "miniRT.h"

// Helper function to print status messages
void display_status(t_scene *scene)
{
	char status[100];

	if (!ft_strncmp(scene->name, "sphere", 6))
	{
		// Simple sphere status
		snprintf(status, 100, "Simple Sphere | Camera: (%.1f, %.1f, %.1f)",
				scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
	}
	// Format status text based on scene type
	else if (scene->is_3d)
	{
		// 3D mode status
		snprintf(status, 100, "3D Mode | Iterations: %d | Resolution: %d",
				scene->menger.iterations, scene->resolution_factor);
	}
	else
	{
		// 2D mode status
		snprintf(status, 100, "scene: %s | Zoom: %.2f | Iterations: %d",
				scene->name, scene->zoom, scene->iterations_defintion);
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