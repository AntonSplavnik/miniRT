#include "miniRT.h"

void draw_checkbox(t_scene *scene)
{
	int x = 50;
	int y = 50;
	int size = 20;
    int i = 0;
    int j = 0;
	
    // Draw checkbox border
    while (i < size)
    {
		mlx_pixel_put(scene->mlx_connection, scene->mlx_window, x + i, y, 0xFFFFFF);
		mlx_pixel_put(scene->mlx_connection, scene->mlx_window, x + i, y + size - 1, 0xFFFFFF);
		mlx_pixel_put(scene->mlx_connection, scene->mlx_window, x, y + i, 0xFFFFFF);
		mlx_pixel_put(scene->mlx_connection, scene->mlx_window, x + size - 1, y + i, 0xFFFFFF);
        i++;
	}
	
	//If checked, draw a simple check mark (using text here)
/*	if (scene->app.checkbox_checked)
		mlx_string_put(scene->mlx_connection, scene->mlx_window, x + 7, y + 13, 0x00FF00, "X"); */

	// If checked, draw a simple check mark (using text here)
	if (scene->app.checkbox_checked) {
        // Draw an X using lines
        while (j < size - 4)
        {
            // Draw diagonal lines from top-left to bottom-right
            mlx_pixel_put(scene->mlx_connection, scene->mlx_window, 
                          x + 2 + j, y + 2 + j, 0x00FF00);
            // Draw diagonal lines from top-right to bottom-left
            mlx_pixel_put(scene->mlx_connection, scene->mlx_window, 
                          x + size - 3 - j, y + 2 + j, 0x00FF00);
            j++;
        }

	}
	  // Add a label (optional)
    mlx_string_put(scene->mlx_connection, scene->mlx_window, 
                  x + size + 10, y + 15, 0xFFFFFF, "Enable Shadows");

}
