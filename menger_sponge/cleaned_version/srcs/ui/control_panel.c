#include "platform.h"

// Helper function specifically for control panel pixels
// This avoids any interference with the main window rendering
void control_pixel_put(t_scene *scene, int x, int y, int color)
{
    // Check boundaries for control panel
    if (x < 0 || x >= 250 || y < 0 || y >= 300)
        return;
        
    int offset = (y * scene->app.control_panel.img.line_len) + 
                (x * (scene->app.control_panel.img.bpp / 8));
                
    // Ensure offset is valid
    if (offset < 0 || offset >= scene->app.control_panel.img.line_len * 300)
        return;
        
    *(unsigned int *)(scene->app.control_panel.img.pixels_ptr + offset) = color;
}

// Init control panel image
void init_control_panel(t_scene *scene)
{
    // Create image for the control panel
    scene->app.control_panel.img.img_ptr = mlx_new_image(scene->mlx_connection, 250, 300);
    scene->app.control_panel.img.pixels_ptr = mlx_get_data_addr(
                                               scene->app.control_panel.img.img_ptr, 
                                               &scene->app.control_panel.img.bpp, 
                                               &scene->app.control_panel.img.line_len, 
                                               &scene->app.control_panel.img.endian);
    scene->app.control_panel.initialized = true;
}

// Draw a checkbox in the image
void draw_option_checkbox(t_scene *scene, int x, int y, bool checked)
{
    int size = 15;
    int i, j;
    
    // Draw checkbox background (dark gray)
    for (i = 1; i < size-1; i++) {
        for (j = 1; j < size-1; j++) {
            control_pixel_put(scene, x + i, y + j, 0x222222);
        }
    }
    
    // Draw checkbox border
    for (i = 0; i < size; i++) {
        control_pixel_put(scene, x + i, y, 0xFFFFFF);
        control_pixel_put(scene, x + i, y + size - 1, 0xFFFFFF);
        control_pixel_put(scene, x, y + i, 0xFFFFFF);
        control_pixel_put(scene, x + size - 1, y + i, 0xFFFFFF);
    }
    
    // Draw check mark if enabled
    if (checked) {
        for (j = 0; j < size - 4; j++) {
            // Draw diagonal lines for X mark
            control_pixel_put(scene, x + 2 + j, y + 2 + j, 0x00FF00);
            control_pixel_put(scene, x + size - 3 - j, y + 2 + j, 0x00FF00);
        }
    }
}

// Draw the entire control panel
void draw_control_panel(t_scene *scene)
{
    int i, j;
    
    // Make sure the control panel is initialized
    if (!scene->app.control_panel.initialized) {
        init_control_panel(scene);
    }
    
    // Clear image with dark gray background
    for (i = 0; i < 250; i++) {
        for (j = 0; j < 300; j++) {
            control_pixel_put(scene, i, j, 0x333333);
        }
    }
    
    // Draw section headers (darker area for the title)
    for (i = 0; i < 250; i++) {
        for (j = 0; j < 40; j++) {
            control_pixel_put(scene, i, j, 0x222222);
        }
    }
    
    // Draw checkboxes
    draw_option_checkbox(scene, 30, 50, scene->app.checkbox_checked);
    draw_option_checkbox(scene, 30, 80, scene->app.enable_reflections);
    draw_option_checkbox(scene, 30, 110, scene->app.enable_specular);
    
    // Add render button
    int button_x = 70;
    int button_y = 200;
    int button_width = 100;
    int button_height = 30;
    
    // Draw button with filled background
    for (i = 0; i < button_width; i++) {
        for (j = 0; j < button_height; j++) {
            control_pixel_put(scene, button_x + i, button_y + j, 0x444444);
        }
    }
    
    // Draw button outline
    for (i = 0; i < button_width; i++) {
        control_pixel_put(scene, button_x + i, button_y, 0xFFFFFF);
        control_pixel_put(scene, button_x + i, button_y + button_height - 1, 0xFFFFFF);
    }
    for (i = 0; i < button_height; i++) {
        control_pixel_put(scene, button_x, button_y + i, 0xFFFFFF);
        control_pixel_put(scene, button_x + button_width - 1, button_y + i, 0xFFFFFF);
    }
    
    // Put the image to the window
    mlx_put_image_to_window(scene->mlx_connection, scene->app.control_window,
                          scene->app.control_panel.img.img_ptr, 0, 0);
    
    // Add title text - we add text after the image to ensure it displays on top
    mlx_string_put(scene->mlx_connection, scene->app.control_window, 
                 80, 25, 0xFFFFFF, "Render Options");
                  
    // Add checkbox labels
    mlx_string_put(scene->mlx_connection, scene->app.control_window, 
                 50, 62, 0xFFFFFF, "Enable Shadows");
    mlx_string_put(scene->mlx_connection, scene->app.control_window, 
                 50, 92, 0xFFFFFF, "Enable Reflections");
    mlx_string_put(scene->mlx_connection, scene->app.control_window, 
                 50, 122, 0xFFFFFF, "Enable Specular");
                 
    // Add button label
    mlx_string_put(scene->mlx_connection, scene->app.control_window, 
                 button_x + 25, button_y + 20, 0xFFFFFF, "Render");
}

// Clean up control panel resources
void destroy_control_panel(t_scene *scene)
{
    if (scene->app.control_panel.initialized) {
        mlx_destroy_image(scene->mlx_connection, scene->app.control_panel.img.img_ptr);
        scene->app.control_panel.initialized = false;
    }
}