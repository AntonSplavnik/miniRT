#include "../../includes/miniRT.h"

// Handle mouse events in the control window
// int control_mouse_handler(int button, int x, int y, t_scene *scene)
// {
//     // Only process left clicks
//     if (button == 1) {
//         // Check shadow checkbox
//         if (x >= 30 && x <= 45 && y >= 50 && y <= 65) {
//             scene->app.enable_hard_shadows = !scene->app.enable_hard_shadows;
//             draw_control_panel(scene);
//             // Re-render with new settings when checkbox is toggled
//             render_scene(scene);
//             return (0);
//         }

//         // Check reflections checkbox
//         if (x >= 30 && x <= 45 && y >= 80 && y <= 95) {
//             scene->app.enable_reflections = !scene->app.enable_reflections;
//             draw_control_panel(scene);
//             render_scene(scene);
//             return (0);
//         }

//         // Check specular checkbox
//         if (x >= 30 && x <= 45 && y >= 110 && y <= 125) {
//             scene->app.enable_specular = !scene->app.enable_specular;
//             draw_control_panel(scene);
//             render_scene(scene);
//             return (0);
//         }

//         // Check if render button was clicked
// /*         if (x >= 70 && x <= 170 && y >= 200 && y <= 230) {
//             // Re-render with new settings
//             render_scene(scene);
//             return (0);
//         } */
//     }
//     return (0);
// }


void mouse_button_callback(mouse_key_t button, action_t action, modifier_key_t mods, void* param)
{
	t_scene *scene;
	// int	x;
	// int y;

	(void)mods;
	scene  = (t_scene *)param;
	

	// Update button state
    if (button == MLX_MOUSE_BUTTON_LEFT) {
        scene->mouse_state.left_button_down = (action == MLX_PRESS);
    } else if (button == MLX_MOUSE_BUTTON_RIGHT) {
        scene->mouse_state.right_button_down = (action == MLX_PRESS);
    } else if (button == MLX_MOUSE_BUTTON_MIDDLE) {
        scene->mouse_state.middle_button_down = (action == MLX_PRESS);
    }

	

	if (action == MLX_PRESS)
	{
		if (button == MLX_MOUSE_BUTTON_RIGHT)
		{
			scene->mouse_state.is_dragging = true;
			scene->mouse_state.right_button_down = true;
			scene->graphic_settings.resolution_factor = 4;

			mlx_get_mouse_pos(scene->app.mlx, &scene->mouse_state.x, &scene->mouse_state.y);
			scene->mouse_state.prev_mouse_x = scene->mouse_state.x;
			scene->mouse_state.prev_mouse_y = scene->mouse_state.y;
		}
	}
	else if (action == MLX_RELEASE)
    	{
			if (button == MLX_MOUSE_BUTTON_RIGHT)
			{
				scene->mouse_state.is_dragging = false;
				scene->mouse_state.right_button_down = false;
				scene->graphic_settings.resolution_factor = 1;
				
				render_scene(scene);
				display_status(scene);
			}
		}
	}


void	cursor_position_callback(double xpos, double ypos, void* param)
{
	t_scene	*scene;
	static double last_render_time = 0;
	double current_time;

	scene = (t_scene *)param;

	// Track mouse position
	scene->mouse_state.x = (int32_t)xpos;
	scene->mouse_state.y = (int32_t)ypos;

	if(scene->mouse_state.right_button_down && scene->mouse_state.is_dragging)
	{
		// Calculate movement deltas with floating point precision
		double dx = (double)scene->mouse_state.x - scene->mouse_state.prev_mouse_x;
		double dy = (double)scene->mouse_state.y - scene->mouse_state.prev_mouse_y;
		
		// Apply movement if there's any change
		if (dx != 0.0 || dy != 0.0)
		{
			// Apply smoother movement with adjusted sensitivity
			scene->camera.rotation.y += dx * 0.005;  // Reduced sensitivity
			scene->camera.rotation.x += dy * 0.005;  // Reduced sensitivity
			
			// Store new positions with floating point precision
			scene->mouse_state.prev_mouse_x = scene->mouse_state.x;
			scene->mouse_state.prev_mouse_y = scene->mouse_state.y;
			
			// Set low resolution for dragging
			scene->graphic_settings.resolution_factor = 4;
			
			// Get current time
			current_time = mlx_get_time();
			
			// Limit rendering to 30 frames per second during dragging
			if (current_time - last_render_time > 0.066) { // ~30 FPS
				render_scene(scene);
				last_render_time = current_time;
			}
		}
	}
}


void	setup_mouse_hook(t_scene *scene)
{
    mlx_mouse_hook(scene->app.mlx, mouse_button_callback, scene);
    mlx_cursor_hook(scene->app.mlx, cursor_position_callback, scene);
    // mlx_scroll_hook(scene->app.mlx, scroll_callback, scene);
}