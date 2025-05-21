#include "miniRT.h"

// Handle mouse events in the control window
int control_mouse_handler(int button, int x, int y, t_scene *scene)
{
    // Only process left clicks
    if (button == 1) {
        // Check shadow checkbox
        if (x >= 30 && x <= 45 && y >= 50 && y <= 65) {
            scene->app.enable_hard_shadows = !scene->app.enable_hard_shadows;
            draw_control_panel(scene);
            // Re-render with new settings when checkbox is toggled
            render_complex_scene(scene);
            return (0);
        }

        // Check reflections checkbox
        if (x >= 30 && x <= 45 && y >= 80 && y <= 95) {
            scene->app.enable_reflections = !scene->app.enable_reflections;
            draw_control_panel(scene);
            render_complex_scene(scene);
            return (0);
        }

        // Check specular checkbox
        if (x >= 30 && x <= 45 && y >= 110 && y <= 125) {
            scene->app.enable_specular = !scene->app.enable_specular;
            draw_control_panel(scene);
            render_complex_scene(scene);
            return (0);
        }

        // Check if render button was clicked
/*         if (x >= 70 && x <= 170 && y >= 200 && y <= 230) {
            // Re-render with new settings
            render_complex_scene(scene);
            return (0);
        } */
    }
    return (0);
}

// Mouse motion handler for 3D fractal rotation
static void handle_3d_rotation(int x, int y, t_scene *scene)
{
	// Calculate movement deltas
	int dx = x - scene->prev_mouse_x;
	int dy = y - scene->prev_mouse_y;

	// Update rotation (scale down the movement for smoother control)
	// Use mouse for rotation, not position adjustment
	scene->camera.rotation.y += dx * 0.01;  // Horizontal mouse movement rotates around Y axis
	scene->camera.rotation.x += dy * 0.01;  // Vertical mouse movement rotates around X axis

	// Store new positions
	scene->prev_mouse_x = x;
	scene->prev_mouse_y = y;

	// Render at low resolution during dragging for better performance
	// Use 4 instead of 8 for better quality during dragging
	scene->app.resolution_factor = 4;
}

int motion_handler(int x, int y, t_scene *scene)
{
    if (scene->is_dragging)
    {
        handle_3d_rotation(x, y, scene);
		render_complex_scene(scene);
    }
    return (0);
}

//Used
int	mouse_handler(int button, int x, int y, t_scene *scene)
{
	(void)x;
	(void)y;

/* 	// Check for checkbox clicks
    if (button == 1) { // Left click
        int cb_x = 50;
        int cb_y = 50;
        int cb_size = 20;

        if (x >= cb_x && x <= cb_x + cb_size && y >= cb_y && y <= cb_y + cb_size) {
            scene->app.enable_hard_shadows = !scene->app.enable_hard_shadows;
			render_complex_scene(scene);

            // mlx_clear_window(scene->mlx_connection, scene->mlx_window);
            // draw_checkbox(scene);
        }
    } */

#ifdef __APPLE__
	if (button == MOUSE_RIGHT_BUTTON)
#else
	if (button == 3)
#endif
   {
		scene->is_dragging = 1;
		scene->prev_mouse_x = x;
		scene->prev_mouse_y = y;
		return(0);
   }
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
	(void)button;
	(void)x;
	(void)y;

#ifdef __APPLE__
	if (button == MOUSE_RIGHT_BUTTON)
#else
	if (button == Button3)
#endif

	if (scene->is_dragging)
	{
		scene->is_dragging = false;

		// Render at full quality after release
		if (scene)
		{
			scene->is_dragging = 0;
			scene->app.resolution_factor = 1;
			render_complex_scene(scene);
			display_status(scene);
		}
	}
	return (0);
}




/*
//from menger 12
int	mouse_handler(int button, int x, int y, t_fractal *fractal)
{
	// Safety check
	if (!fractal)
	{
		return (0);
	}

	// For 3D fractal types
	if (fractal->is_3d)
	{
		if (button == 1) // Left mouse button
		{
			fractal->is_dragging = 1;
			fractal->prev_mouse_x = x;
			fractal->prev_mouse_y = y;
			return (0);
		}
		else if (button == 4) // Mouse wheel up
		{
			// Zoom in (move camera forward)
			fractal->camera.position.z += 0.3;

			if (!ft_strncmp(fractal->name, "menger", 6))
			{
				// Use a lower resolution for preview during movement
				fractal->resolution_factor = 4;
				render_menger_sponge(fractal);
			}
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
			{
				// Use a lower resolution for preview during movement
				fractal->resolution_factor = 4;
				render_mandelbrot3d(fractal);
			}

			display_status(fractal);
			return (0);
		}
		else if (button == 5) // Mouse wheel down
		{
			// Zoom out (move camera backward)
			fractal->camera.position.z -= 0.3;

			if (!ft_strncmp(fractal->name, "menger", 6))
			{
				// Use a lower resolution for preview during movement
				fractal->resolution_factor = 4;
				render_menger_sponge(fractal);
			}
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
			{
				// Use a lower resolution for preview during movement
				fractal->resolution_factor = 4;
				render_mandelbrot3d(fractal);
			}

			display_status(fractal);
			return (0);
		}
	}
	return (0);
}
*/
/*
int	mouse_release(int button, int x, int y, t_fractal *fractal)
{
	(void)button;
	(void)x;
	(void)y;

	if (fractal->is_dragging)
	{
		fractal->is_dragging = 0;

		// If we were dragging in 3D mode, render at full quality after release
		if (fractal->is_3d)
		{
			fractal->resolution_factor = 1;

			if (!ft_strncmp(fractal->name, "menger", 6))
				render_menger_sponge(fractal);
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
				render_mandelbrot3d(fractal);

			display_status(fractal);
		}
	}

	return (0);
}
*/
