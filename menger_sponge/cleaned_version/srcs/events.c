/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:08:43 by asplavni          #+#    #+#             */
/*   Updated: 2025/05/05 10:39:42 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"
#include <stdio.h>

//Used
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

//Used
int	close_handler(t_scene *scene)
{
	static int	freed = 0;

	if (freed)
		return (0);
	freed = 1;

	cleanup_scene(scene);

	// Free BVH for Menger sponge if it exists
	if (!ft_strncmp(scene->name, "menger", 6) && scene->menger.bvh_root)
	{
		free_bvh(scene->menger.bvh_root);
		scene->menger.bvh_root = NULL;
	}

	// Clear all other resources
	if (scene->img.img_ptr && scene->mlx_connection)
	{
		mlx_destroy_image(scene->mlx_connection, scene->img.img_ptr);
		scene->img.img_ptr = NULL;
	}

	if (scene->mlx_window && scene->mlx_connection)
	{
		mlx_destroy_window(scene->mlx_connection, scene->mlx_window);
		scene->mlx_window = NULL;
	}

	// Free the MLX pointer too
	if (scene->mlx_connection)
	{
#ifdef __linux__
		mlx_destroy_display(scene->mlx_connection);
#endif
		free(scene->mlx_connection);
		scene->mlx_connection = NULL;
	}

	exit(EXIT_SUCCESS);
	// Exit the program cleanly
	return (0);
}

//Used
int	key_handler(int keysym, t_scene *scene)
{
#ifdef __APPLE__
	if (keysym == KEY_ESC)
#else
	if (keysym == XK_Escape)
#endif
	{
		// Call close_handler to clean up resources
		close_handler(scene);
		// Exit directly since we can't end the mlx loop gracefully
		exit(EXIT_SUCCESS);
	}
	// Handle camera movement for the sphere renderer
	int camera_changed = 0;
	// Camera movement - slower to be more precise
#ifdef __APPLE__
	if (keysym == KEY_W)
	{
		scene->camera.position.z += 0.5;
		camera_changed = 1;
	}
	else if (keysym == KEY_S)
	{
		scene->camera.position.z -= 0.5;
		camera_changed = 1;
	}
	else if (keysym == KEY_A)
	{
		scene->camera.position.x -= 0.5;
		camera_changed = 1;
	}
	else if (keysym == KEY_D)
	{
		scene->camera.position.x += 0.5;
		camera_changed = 1;
	}
	else if (keysym == KEY_Q)
	{
		scene->camera.position.y += 0.5;
		camera_changed = 1;
	}
	else if (keysym == KEY_E)
	{
		scene->camera.position.y -= 0.5;
		camera_changed = 1;
	}
	else if (keysym == KEY_LEFT)
	{
		scene->camera.rotation.y -= 0.05;
		camera_changed = 1;
	}
	else if (keysym == KEY_RIGHT)
	{
		scene->camera.rotation.y += 0.05;
		camera_changed = 1;
	}
	else if (keysym == KEY_UP)
	{
		scene->camera.rotation.x -= 0.05;
		camera_changed = 1;
	}
	else if (keysym == KEY_DOWN)
	{
		scene->camera.rotation.x += 0.05;
		camera_changed = 1;
	}
#else
	if (keysym == XK_w)
	{
		scene->camera.position.z += 0.5;
		camera_changed = 1;
	}
	else if (keysym == XK_s)
	{
		scene->camera.position.z -= 0.5;
		camera_changed = 1;
	}
	else if (keysym == XK_a)
	{
		scene->camera.position.x -= 0.5;
		camera_changed = 1;
	}
	else if (keysym == XK_d)
	{
		scene->camera.position.x += 0.5;
		camera_changed = 1;
	}
	else if (keysym == XK_q)
	{
		scene->camera.position.y += 0.5;
		camera_changed = 1;
	}
	else if (keysym == XK_e)
	{
		scene->camera.position.y -= 0.5;
		camera_changed = 1;
	}
	else if (keysym == XK_Left)
	{
		scene->camera.rotation.y -= 0.05;
		camera_changed = 1;
	}
	else if (keysym == XK_Right)
	{
		scene->camera.rotation.y += 0.05;
		camera_changed = 1;
	}
	else if (keysym == XK_Up)
	{
		scene->camera.rotation.x -= 0.05;
		camera_changed = 1;
	}
	else if (keysym == XK_Down)
	{
		scene->camera.rotation.x += 0.05;
		camera_changed = 1;
	}
#endif

	// Re-render if camera has changed
	if (camera_changed)
	{
		if (!ft_strncmp(scene->name, "sphere", 6))
			render_two_spheres(scene);
		else if (!ft_strncmp(scene->name, "menger", 6))
			render_menger_sponge(scene);
		return (0);
	}
	// Reset camera position
#ifdef __APPLE__
	else if (keysym == KEY_r)
#else
	else if (keysym == XK_r)
#endif
	{
		scene->camera.position = (t_vec3){0.0, 0.0, -5.0};
		if (!ft_strncmp(scene->name, "sphere", 6))
			render_two_spheres(scene);
		else if (!ft_strncmp(scene->name, "menger", 6))
			render_menger_sponge(scene);
		return (0);
	}
	#ifdef __APPLE__
		else if (keysym == KEY_PLUS || keysym == KEY_EQUAL)
#else
		else if (keysym == XK_plus || keysym == XK_equal)
#endif
		{
			if (!ft_strncmp(scene->name, "menger", 6))
			{
				if (scene->menger.iterations < 10)
				{
					// Show loading message before reconstruction
					display_progress(scene, "Rebuilding Menger sponge...");
					draw_image_to_window(scene);

					// Update iterations and rebuild BVH
					scene->menger.iterations++;
					if (scene->menger.bvh_root)
						free_bvh(scene->menger.bvh_root);
					scene->menger.bvh_root = build_menger_bvh(scene->menger.iterations);
					if (!scene->menger.bvh_root)
					{
						scene->menger.iterations--;
					}

					// Use direct Menger rendering instead of generic scene_render
					render_menger_sponge(scene);
				}
			}
		}
#ifdef __APPLE__
		else if (keysym == KEY_MINUS)
#else
		else if (keysym == XK_minus)
#endif
		{
			if (!ft_strncmp(scene->name, "menger", 6))
			{
				if (scene->menger.iterations > 0)
				{
					// Show loading message before reconstruction
					display_progress(scene, "Rebuilding Menger sponge...");
					draw_image_to_window(scene);

					// Update iterations and rebuild BVH
					scene->menger.iterations--;
					if (scene->menger.bvh_root)
						free_bvh(scene->menger.bvh_root);
					scene->menger.bvh_root = build_menger_bvh(scene->menger.iterations);
					if (!scene->menger.bvh_root)
					{
						scene->menger.iterations++;
					}

					// Use direct Menger rendering instead of generic scene_render
					render_menger_sponge(scene);
				}
			}
		}
		// Resolution control for performance
#ifdef __APPLE__
		else if (keysym == KEY_bracketleft && scene->resolution_factor < 16)
#else
		else if (keysym == XK_bracketleft && scene->resolution_factor < 16)
#endif
		{
			if (!ft_strncmp(scene->name, "menger", 6))
			{
				scene->resolution_factor *= 2;  // Lower resolution (faster)
				render_menger_sponge(scene);    // Re-render with new resolution
			}
		}
#ifdef __APPLE__
		else if (keysym == KEY_bracketright && scene->resolution_factor > 1)
#else
		else if (keysym == XK_bracketright && scene->resolution_factor > 1)
#endif
		{
			if (!ft_strncmp(scene->name, "menger", 6))
			{
				scene->resolution_factor /= 2;  // Higher resolution (slower)
				render_menger_sponge(scene);    // Re-render with new resolution
			}
		}
		// Reset camera position
#ifdef __APPLE__
		else if (keysym == KEY_r)
#else
		else if (keysym == XK_r)
#endif
		{
			scene->camera.position = (t_vec3){0, 0, -3};
			scene->camera.rotation = (t_vec3){0, 0, 0};
			if (!ft_strncmp(scene->name, "menger", 6))
				render_menger_sponge(scene);
			else if (!ft_strncmp(scene->name, "sphere", 6))
				render_two_spheres(scene);
		}
		// Debug camera positions
#ifdef __APPLE__
		else if (keysym == KEY_1)
#else
		else if (keysym == XK_1)
#endif
		{
			scene->camera.position = (t_vec3){0.0, 0.0, -4.0};
			scene->camera.rotation = (t_vec3){0.0, 0.0, 0.0};
			if (!ft_strncmp(scene->name, "menger", 6))
				render_menger_sponge(scene);
			else if (!ft_strncmp(scene->name, "sphere", 6))
				render_two_spheres(scene);
		}
#ifdef __APPLE__
		else if (keysym == KEY_2)
#else
		else if (keysym == XK_2)
#endif
		{
			scene->camera.position = (t_vec3){4.0, 0.0, 0.0};
			scene->camera.rotation = (t_vec3){0.0, -1.57, 0.0};
			if (!ft_strncmp(scene->name, "menger", 6))
				render_menger_sponge(scene);
			else if (!ft_strncmp(scene->name, "sphere", 6))
				render_two_spheres(scene);
		}
#ifdef __APPLE__
		else if (keysym == KEY_3)
#else
		else if (keysym == XK_3)
#endif
		{
			scene->camera.position = (t_vec3){0.0, 4.0, -0.0};
			scene->camera.rotation = (t_vec3){1.6, 0.0, 0.0};
			if (!ft_strncmp(scene->name, "menger", 6))
				render_menger_sponge(scene);
			else if (!ft_strncmp(scene->name, "sphere", 6))
				render_two_spheres(scene);
		}
#ifdef __APPLE__
		else if (keysym == KEY_4)
#else
		else if (keysym == XK_4)
#endif
		{
			scene->camera.position = (t_vec3){2.4, 0.4, -2.8};
			scene->camera.rotation = (t_vec3){0.15, -0.7, 0.0};
			if (!ft_strncmp(scene->name, "menger", 6))
				render_menger_sponge(scene);
			else if (!ft_strncmp(scene->name, "sphere", 6))
				render_two_spheres(scene);
		}
#ifdef __APPLE__
		else if (keysym == KEY_5)
#else
		else if (keysym == XK_5)
#endif
		{
			scene->camera.position = (t_vec3){2.4, 2.0, -2.8};
			scene->camera.rotation = (t_vec3){0.45, -0.7, 0.0};
			if (!ft_strncmp(scene->name, "menger", 6))
				render_menger_sponge(scene);
			else if (!ft_strncmp(scene->name, "sphere", 6))
				render_two_spheres(scene);
		}

		// Ensure we're in 3D mode and render
		scene->is_3d = 1;
		return (0);

//	else if (!ft_strncmp(scene->name, "menger", 6))
//	{
//		int camera_changed = 0;
//		// Camera movement - slower to be more precise
//#ifdef __APPLE__
//		if (keysym == KEY_W)
//		{
//			scene->camera.position.z += 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_S)
//		{
//			scene->camera.position.z -= 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_A)
//		{
//			scene->camera.position.x -= 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_D)
//		{
//			scene->camera.position.x += 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_Q)
//		{
//			scene->camera.position.y += 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_E)
//		{
//			scene->camera.position.y -= 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_LEFT)
//		{
//			scene->camera.rotation.y -= 0.05;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_RIGHT)
//		{
//			scene->camera.rotation.y += 0.05;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_UP)
//		{
//			scene->camera.rotation.x -= 0.05;
//			camera_changed = 1;
//		}
//		else if (keysym == KEY_DOWN)
//		{
//			scene->camera.rotation.x += 0.05;
//			camera_changed = 1;
//		}
//#else
//		if (keysym == XK_w)
//		{
//			scene->camera.position.z += 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_s)
//		{
//			scene->camera.position.z -= 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_a)
//		{
//			scene->camera.position.x -= 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_d)
//		{
//			scene->camera.position.x += 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_q)
//		{
//			scene->camera.position.y += 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_e)
//		{
//			scene->camera.position.y -= 0.2;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_Left)
//		{
//			scene->camera.rotation.y -= 0.05;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_Right)
//		{
//			scene->camera.rotation.y += 0.05;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_Up)
//		{
//			scene->camera.rotation.x -= 0.05;
//			camera_changed = 1;
//		}
//		else if (keysym == XK_Down)
//		{
//			scene->camera.rotation.x += 0.05;
//			camera_changed = 1;
//		}
//#endif

//		// Re-render if camera has changed
//		if (camera_changed)
//		{
//			render_menger_sponge(scene);
//			return (0);
//		}
//		// Change Menger sponge iterations
//#ifdef __APPLE__
//		else if (keysym == KEY_PLUS || keysym == KEY_EQUAL)
//#else
//		else if (keysym == XK_plus || keysym == XK_equal)
//#endif
//		{
//			if (scene->menger.iterations < 10)
//			{
//				// Show loading message before reconstruction
//				display_progress(scene, "Rebuilding Menger sponge...");
//				draw_image_to_window(scene);

//				// Update iterations and rebuild BVH
//				scene->menger.iterations++;
//				if (scene->menger.bvh_root)
//					free_bvh(scene->menger.bvh_root);
//				scene->menger.bvh_root = build_menger_bvh(scene->menger.iterations);
//				if (!scene->menger.bvh_root)
//				{
//					scene->menger.iterations--;
//				}

//				// Use direct Menger rendering instead of generic scene_render
//				render_menger_sponge(scene);
//			}
//		}
//#ifdef __APPLE__
//		else if (keysym == KEY_MINUS)
//#else
//		else if (keysym == XK_minus)
//#endif
//		{
//			if (scene->menger.iterations > 0)
//			{
//				// Show loading message before reconstruction
//				display_progress(scene, "Rebuilding Menger sponge...");
//				draw_image_to_window(scene);

//				// Update iterations and rebuild BVH
//				scene->menger.iterations--;
//				if (scene->menger.bvh_root)
//					free_bvh(scene->menger.bvh_root);
//				scene->menger.bvh_root = build_menger_bvh(scene->menger.iterations);
//				if (!scene->menger.bvh_root)
//				{
//					scene->menger.iterations++;
//				}

//				// Use direct Menger rendering instead of generic scene_render
//				render_menger_sponge(scene);
//			}
//		}
//		// Resolution control for performance
//#ifdef __APPLE__
//		else if (keysym == KEY_bracketleft && scene->resolution_factor < 16)
//#else
//		else if (keysym == XK_bracketleft && scene->resolution_factor < 16)
//#endif
//		{
//			scene->resolution_factor *= 2;  // Lower resolution (faster)
//			render_menger_sponge(scene);    // Re-render with new resolution
//		}
//#ifdef __APPLE__
//		else if (keysym == KEY_bracketright && scene->resolution_factor > 1)
//#else
//		else if (keysym == XK_bracketright && scene->resolution_factor > 1)
//#endif
//		{
//			scene->resolution_factor /= 2;  // Higher resolution (slower)
//			render_menger_sponge(scene);    // Re-render with new resolution
//		}
//		// Reset camera position
//#ifdef __APPLE__
//		else if (keysym == KEY_r)
//#else
//		else if (keysym == XK_r)
//#endif
//		{
//			scene->camera.position = (t_vec3){0, 0, -3};
//			scene->camera.rotation = (t_vec3){0, 0, 0};
//			render_menger_sponge(scene);
//		}
//		// Debug camera positions
//#ifdef __APPLE__
//		else if (keysym == KEY_1)
//#else
//		else if (keysym == XK_1)
//#endif
//		{
//			scene->camera.position = (t_vec3){0.0, 0.0, -4.0};
//			scene->camera.rotation = (t_vec3){0.0, 0.0, 0.0};
//			render_menger_sponge(scene);
//		}
//#ifdef __APPLE__
//		else if (keysym == KEY_2)
//#else
//		else if (keysym == XK_2)
//#endif
//		{
//			scene->camera.position = (t_vec3){4.0, 0.0, 0.0};
//			scene->camera.rotation = (t_vec3){0.0, -1.57, 0.0};
//			render_menger_sponge(scene);
//		}
//#ifdef __APPLE__
//		else if (keysym == KEY_3)
//#else
//		else if (keysym == XK_3)
//#endif
//		{
//			scene->camera.position = (t_vec3){0.0, 4.0, -0.0};
//			scene->camera.rotation = (t_vec3){1.6, 0.0, 0.0};
//			render_menger_sponge(scene);
//		}
//#ifdef __APPLE__
//		else if (keysym == KEY_4)
//#else
//		else if (keysym == XK_4)
//#endif
//		{
//			scene->camera.position = (t_vec3){2.4, 0.4, -2.8};
//			scene->camera.rotation = (t_vec3){0.15, -0.7, 0.0};
//			render_menger_sponge(scene);
//		}
//#ifdef __APPLE__
//		else if (keysym == KEY_5)
//#else
//		else if (keysym == XK_5)
//#endif
//		{
//			scene->camera.position = (t_vec3){2.4, 2.0, -2.8};
//			scene->camera.rotation = (t_vec3){0.45, -0.7, 0.0};
//			render_menger_sponge(scene);
//		}

//		// Ensure we're in 3D mode and render
//		scene->is_3d = 1;
//		// The camera position keys will handle rendering when needed
//		return (0);
//	}
//	return (0);
}

//Used
int	mouse_handler(int button, int x, int y, t_scene *scene)
{
	(void)x;
	(void)y;
	// For 3D mode - handle camera controls
	if (scene->is_3d)
	{
		// Adjust camera position based on mouse scroll
#ifdef __APPLE__
		if (button == MOUSE_SCROLL_DOWN)  // Zoom out
		{
			scene->camera.position.z *= 1.1;
		}
		else if (button == MOUSE_SCROLL_UP)  // Zoom in
		{
			scene->camera.position.z *= 0.9;
		}
#else
		if (button == Button5)  // Zoom out
		{
			scene->camera.position.z *= 1.1;
		}
		else if (button == Button4)  // Zoom in
		{
			scene->camera.position.z *= 0.9;
		}
#endif

		render_menger_sponge(scene);
		return (0);
	}
	return (0);
}

//Used
int	mouse_release(int button, int x, int y, t_scene *scene)
{
	(void)x;
	(void)y;
#ifdef __APPLE__
	if (button == MOUSE_LEFT_BUTTON)
#else
	if (button == Button1)
#endif
		scene->is_dragging = 0;
	return (0);
}
