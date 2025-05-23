#include "miniRT.h"


int	key_hendler(mlx_key_data_t keydata, t_scene *scene)
{

}

//Used
int key_handler_mlx42(mlx_Key_data_t keydata, void *param)
{
	t_scene *scene = (t_scene *)param;


	// Only handle key press events (not repeat or release)
    if (keydata.action != MLX_PRESS)
        return;
    
    // Handle escape key - close window
    if (keydata.key == MLX_KEY_ESCAPE)
    {
        // Call close_handler to clean up resources
        close_handler(scene);
        // MLX42 handles exit gracefully
        mlx_close_window(scene->mlx);
        return;
    }

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
		t_vec3 forward = get_forward_vector(scene->camera.rotation);
		scene->camera.position = vec3_add(scene->camera.position, vec3_scale(forward, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == KEY_S)
	{
		t_vec3 forward = get_forward_vector(scene->camera.rotation);
		scene->camera.position = vec3_subtract(scene->camera.position, vec3_scale(forward, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == KEY_A)
	{
		t_vec3 right = get_right_vector(scene->camera.rotation);
		scene->camera.position = vec3_subtract(scene->camera.position, vec3_scale(right, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == KEY_D)
	{
		t_vec3 right = get_right_vector(scene->camera.rotation);
		scene->camera.position = vec3_add(scene->camera.position, vec3_scale(right, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == KEY_Q)
	{
		t_vec3 up = get_up_vector(scene->camera.rotation);
		scene->camera.position = vec3_add(scene->camera.position, vec3_scale(up, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == KEY_E)
	{
		t_vec3 up = get_up_vector(scene->camera.rotation);
		scene->camera.position = vec3_subtract(scene->camera.position, vec3_scale(up, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == KEY_LEFT)
	{
		scene->camera.rotation.y -= scene->camera.rotation_speed;
		camera_changed = 1;
	}
	else if (keysym == KEY_RIGHT)
	{
		scene->camera.rotation.y += scene->camera.rotation_speed;
		camera_changed = 1;
	}
	else if (keysym == KEY_UP)
	{
		scene->camera.rotation.x -= scene->camera.rotation_speed;
		camera_changed = 1;
	}
	else if (keysym == KEY_DOWN)
	{
		scene->camera.rotation.x += scene->camera.rotation_speed;
		camera_changed = 1;
	}
#else
	if (keysym == XK_w)
	{
		t_vec3 forward = get_forward_vector(scene->camera.rotation);
		scene->camera.position = vec3_add(scene->camera.position, vec3_scale(forward, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == XK_s)
	{
		t_vec3 forward = get_forward_vector(scene->camera.rotation);
		scene->camera.position = vec3_subtract(scene->camera.position, vec3_scale(forward, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == XK_a)
	{
		t_vec3 right = get_right_vector(scene->camera.rotation);
		scene->camera.position = vec3_subtract(scene->camera.position, vec3_scale(right, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == XK_d)
	{
		t_vec3 right = get_right_vector(scene->camera.rotation);
		scene->camera.position = vec3_add(scene->camera.position, vec3_scale(right, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == XK_q)
	{
		t_vec3 up = get_up_vector(scene->camera.rotation);
		scene->camera.position = vec3_add(scene->camera.position, vec3_scale(up, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == XK_e)
	{
		t_vec3 up = get_up_vector(scene->camera.rotation);
		scene->camera.position = vec3_subtract(scene->camera.position, vec3_scale(up, scene->camera.movement_speed));
		camera_changed = 1;
	}
	else if (keysym == XK_Left)
	{
		scene->camera.rotation.y -= scene->camera.rotation_speed;
		camera_changed = 1;
	}
	else if (keysym == XK_Right)
	{
		scene->camera.rotation.y += scene->camera.rotation_speed;
		camera_changed = 1;
	}
	else if (keysym == XK_Up)
	{
		scene->camera.rotation.x -= scene->camera.rotation_speed;
		camera_changed = 1;
	}
	else if (keysym == XK_Down)
	{
		scene->camera.rotation.x += scene->camera.rotation_speed;
		camera_changed = 1;
	}
#endif

	// Re-render if camera has changed
	if (camera_changed)
	{
		render_complex_scene(scene);
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
		render_complex_scene(scene);
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
			render_complex_scene(scene);
		}
		// Debug camera positions
#ifdef __APPLE__
		else if (keysym == KEY_1)
#else
		else if (keysym == XK_1) //Frontal view
#endif
		{
			scene->camera.position = (t_vec3){0.0, 0.0, -4.0};
			scene->camera.rotation = (t_vec3){0.0, 0.0, 0.0};
			render_complex_scene(scene);
		}
#ifdef __APPLE__
		else if (keysym == KEY_2)
#else
		else if (keysym == XK_2) //Side view(right)
#endif
		{
			scene->camera.position = (t_vec3){4.0, 0.0, 0.0}; //camera on the right side
			scene->camera.rotation = (t_vec3){0.0, -1.57, 0.0}; //-90 degrees angle around Y

			render_complex_scene(scene);
		}
#ifdef __APPLE__
		else if (keysym == KEY_3)
#else
		else if (keysym == XK_3) //top view
#endif
		{
			scene->camera.position = (t_vec3){0.0, 4.0, 2.0};
			scene->camera.rotation = (t_vec3){1.57, 0.0, 0.0};
			render_complex_scene(scene);
		}
#ifdef __APPLE__
		else if (keysym == KEY_4)
#else
		else if (keysym == XK_4)
#endif
		{
			scene->camera.position = (t_vec3){2.4, 0.4, -2.8};
			scene->camera.rotation = (t_vec3){0.15, -0.7, 0.0};
			render_complex_scene(scene);
		}
#ifdef __APPLE__
		else if (keysym == KEY_5)
#else
		else if (keysym == XK_5)
#endif
		{
			scene->camera.position = (t_vec3){2.4, 2.0, -2.8};
			scene->camera.rotation = (t_vec3){0.45, -0.7, 0.0};
			render_complex_scene(scene);
		}
#ifdef __APPLE__
else if (keysym == KEY_6)
#else
else if (keysym == XK_6) //Side view (left)
	#endif
		{
			scene->camera.position = (t_vec3){-4.0, 0.0, 1.0}; //camera on the left side
			scene->camera.rotation = (t_vec3){0.0, 1.57, 0.0};; // 90 degrees angle around Y
			render_complex_scene(scene);
		}
#ifdef __APPLE__
else if (keysym == KEY_7)
#else
else if (keysym == XK_7) //Bottom view
#endif
		{
			// Position to look at the bottom cap
			scene->camera.position = vec3_create(0.0, -4.0, 2.0);
			scene->camera.rotation = vec3_create(-1.57, 0.0, 0.0); // Look straight up
			render_complex_scene(scene);
		}

		// Ensure we're in 3D mode and render
		scene->is_3d = 1;
		return (0);
}
