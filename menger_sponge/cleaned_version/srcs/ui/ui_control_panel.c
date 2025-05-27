#include "../../includes/miniRT.h"

// UI Panel state
typedef struct s_ui_panel {
    bool visible;            // Is the panel currently visible
    int current_width;       // Current width during animation
    int target_width;        // Target width for animation
    int height;              // Panel height
    int x;                   // Panel X position
    int y;                   // Panel Y position
    int toggle_button_size;  // Size of the toggle button
    int animation_speed;     // Speed of slide animation
    int padding;             // Padding inside panel
    int checkbox_size;       // Size of checkboxes
    bool text_initialized;   // Flag to track if text has been initialized
    mlx_image_t *panel_img;  // Dedicated image for UI panel
} t_ui_panel;

// Global UI panel state
static t_ui_panel g_panel = {
    .visible = false,
    .current_width = 0,
    .target_width = 250,
    .height = 400,
    .x = 0,
    .y = 50,
    .toggle_button_size = 30,
    .animation_speed = 10,  // Reduced for smoother animation
    .padding = 15,
    .checkbox_size = 15,
    .text_initialized = false,
    .panel_img = NULL
};

/**
 * Clear the entire panel area to ensure no artifacts remain
 */
static void clear_panel_area(t_scene *scene)
{
    // Clear the entire panel image with transparency
    if (g_panel.panel_img) {
        for (uint32_t y = 0; y < g_panel.panel_img->height; y++) {
            for (uint32_t x = 0; x < g_panel.panel_img->width; x++) {
                mlx_put_pixel(g_panel.panel_img, x, y, 0x00000000); // Fully transparent
            }
        }
    }
}

/**
 * Draw a checkbox on the image
 */
static void draw_checkbox(mlx_image_t *img, int x, int y, bool checked)
{
    int size = g_panel.checkbox_size;
    int i, j;
    uint32_t bg_color = 0x222222FF;       // Solid color for better visibility
    uint32_t border_color = 0xFFFFFFFF;
    uint32_t check_color = 0x00FF00FF;

    // Draw checkbox background (dark gray)
    for (i = 1; i < size-1; i++) {
        for (j = 1; j < size-1; j++) {
            mlx_put_pixel(img, x + i, y + j, bg_color);
        }
    }

    // Draw checkbox border
    for (i = 0; i < size; i++) {
        mlx_put_pixel(img, x + i, y, border_color);
        mlx_put_pixel(img, x + i, y + size - 1, border_color);
        mlx_put_pixel(img, x, y + i, border_color);
        mlx_put_pixel(img, x + size - 1, y + i, border_color);
    }

    // Draw check mark if enabled
    if (checked) {
        for (j = 0; j < size - 4; j++) {
            // Draw diagonal lines for X mark
            mlx_put_pixel(img, x + 2 + j, y + 2 + j, check_color);
            mlx_put_pixel(img, x + size - 3 - j, y + 2 + j, check_color);
        }
    }
}

/**
 * Draw the toggle button to show/hide the panel
 */
static void draw_toggle_button(t_scene *scene)
{
    int size = g_panel.toggle_button_size;
    int x = 10;
    int y = 10;
    uint32_t button_color = 0x333333FF;  // Solid color for better visibility
    uint32_t border_color = 0xFFFFFFFF;
    uint32_t icon_color = 0xFFFFFFFF;
    
    // Draw button background
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mlx_put_pixel(g_panel.panel_img, x + i, y + j, button_color);
        }
    }
    
    // Draw button border
    for (int i = 0; i < size; i++) {
        mlx_put_pixel(g_panel.panel_img, x + i, y, border_color);
        mlx_put_pixel(g_panel.panel_img, x + i, y + size - 1, border_color);
        mlx_put_pixel(g_panel.panel_img, x, y + i, border_color);
        mlx_put_pixel(g_panel.panel_img, x + size - 1, y + i, border_color);
    }
    
    // Draw settings icon (three horizontal lines)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < size - 10; j++) {
            mlx_put_pixel(g_panel.panel_img, x + 5 + j, y + 8 + (i * 6), icon_color);
        }
    }
}

/**
 * Draw the panel background with semi-transparency
 */
static void draw_panel_background(t_scene *scene)
{
    uint32_t bg_color = 0x333333EE;       // Darker gray with less transparency
    uint32_t header_color = 0x222222EE;   // Darker gray with less transparency
    uint32_t border_color = 0xFFFFFFFF;   // White border
    
    // Don't draw if panel width is 0
    if (g_panel.current_width <= 0)
        return;
    
    // Draw main background
    for (int x = 0; x < g_panel.current_width; x++) {
        for (int y = g_panel.y; y < g_panel.y + g_panel.height; y++) {
            mlx_put_pixel(g_panel.panel_img, x, y, bg_color);
        }
    }
    
    // Draw header area
    for (int x = 0; x < g_panel.current_width; x++) {
        for (int y = g_panel.y; y < g_panel.y + 40; y++) {
            mlx_put_pixel(g_panel.panel_img, x, y, header_color);
        }
    }
    
    // Draw border
    for (int x = 0; x < g_panel.current_width; x++) {
        mlx_put_pixel(g_panel.panel_img, x, g_panel.y, border_color);
        mlx_put_pixel(g_panel.panel_img, x, g_panel.y + g_panel.height - 1, border_color);
    }
    
    for (int y = g_panel.y; y < g_panel.y + g_panel.height; y++) {
        if (g_panel.current_width > 0)
            mlx_put_pixel(g_panel.panel_img, 0, y, border_color);
        if (g_panel.current_width > 1)
            mlx_put_pixel(g_panel.panel_img, g_panel.current_width - 1, y, border_color);
    }
}

/**
 * Draw text on the panel image
 */
static void draw_panel_text(t_scene *scene)
{
    // Only draw text if panel is fully visible to prevent blurring during animation
    if (g_panel.current_width < g_panel.target_width)
        return;
    
    // Get the approximate width of the title text (roughly 8 pixels per character)
    int title_width = 15 * 8; // "Render Settings" is roughly 15 characters
    int title_x = (g_panel.target_width - title_width) / 2; // Center the title
    
    // Panel title - centered
    mlx_put_string(scene->app.mlx, "Render Settings", title_x, g_panel.y + 15);
    
    // Draw checkbox labels - aligned with checkboxes
    int label_x = g_panel.padding + g_panel.checkbox_size + 10;
    int base_y = g_panel.y + 50;
    int spacing = 35;
    
    // Align text vertically with checkboxes (center of checkbox)
    int text_y_offset = (g_panel.checkbox_size - 16) / 2; // 16 is approx text height
    
    mlx_put_string(scene->app.mlx, "Shadows", label_x, base_y + text_y_offset);
    mlx_put_string(scene->app.mlx, "Reflections", label_x, base_y + spacing + text_y_offset);
    mlx_put_string(scene->app.mlx, "Specular", label_x, base_y + spacing * 2 + text_y_offset);
    mlx_put_string(scene->app.mlx, "Refraction", label_x, base_y + spacing * 3 + text_y_offset);
    
    // Resolution label
    int res_y = base_y + spacing * 4 + 10;
    mlx_put_string(scene->app.mlx, "Resolution:", g_panel.padding, res_y);
    
    // Resolution value
    char res_text[20];
    sprintf(res_text, "1/%d", scene->graphic_settings.resolution_factor);
    mlx_put_string(scene->app.mlx, res_text, g_panel.padding + 100, res_y);
    
    g_panel.text_initialized = true;
}

/**
 * Draw the panel content (checkboxes and labels)
 */
static void draw_panel_content(t_scene *scene)
{
    int checkbox_x = g_panel.padding;
    int base_y = g_panel.y + 50;
    int spacing = 35;
    
    // Only draw content if panel is at least half visible
    if (g_panel.current_width < g_panel.target_width / 2) {
        return;
    }
    
    // Draw checkboxes directly on the panel image
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y, scene->graphic_settings.enable_hard_shadows);
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y + spacing, scene->graphic_settings.enable_reflections);
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y + spacing * 2, scene->graphic_settings.enable_specular);
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y + spacing * 3, scene->graphic_settings.enable_refraction);
    
    // Draw render button
    int button_y = base_y + spacing * 5 + 20;
    int button_width = 100;
    int button_height = 30;
    
    // Center the button horizontally in the panel
    int button_x = (g_panel.target_width - button_width) / 2;
    
    // Button background
    for (int i = 0; i < button_width; i++) {
        for (int j = 0; j < button_height; j++) {
            mlx_put_pixel(g_panel.panel_img, button_x + i, button_y + j, 0x444444FF);
        }
    }
    
    // Button border
    for (int i = 0; i < button_width; i++) {
        mlx_put_pixel(g_panel.panel_img, button_x + i, button_y, 0xFFFFFFFF);
        mlx_put_pixel(g_panel.panel_img, button_x + i, button_y + button_height - 1, 0xFFFFFFFF);
    }
    
    for (int j = 0; j < button_height; j++) {
        mlx_put_pixel(g_panel.panel_img, button_x, button_y + j, 0xFFFFFFFF);
        mlx_put_pixel(g_panel.panel_img, button_x + button_width - 1, button_y + j, 0xFFFFFFFF);
    }
    
    // Draw text if panel is fully visible
    draw_panel_text(scene);
}

/**
 * Update the panel animation state
 * Returns true if animation is still in progress
 */
static bool update_panel_animation(void)
{
    if (g_panel.visible) {
        if (g_panel.current_width < g_panel.target_width) {
            g_panel.current_width += g_panel.animation_speed;
            if (g_panel.current_width > g_panel.target_width)
                g_panel.current_width = g_panel.target_width;
            return true;
        }
    } else {
        if (g_panel.current_width > 0) {
            g_panel.current_width -= g_panel.animation_speed;
            if (g_panel.current_width < 0)
                g_panel.current_width = 0;
            return true;
        }
    }
    return false;
}

/**
 * Check if a point is inside the toggle button
 */
static bool is_point_in_toggle_button(int x, int y)
{
    return (x >= 10 && x <= 10 + g_panel.toggle_button_size &&
            y >= 10 && y <= 10 + g_panel.toggle_button_size);
}

/**
 * Check if a point is inside a checkbox at the given position
 */
static bool is_point_in_checkbox(int x, int y, int checkbox_x, int checkbox_y)
{
    return (x >= checkbox_x && x <= checkbox_x + g_panel.checkbox_size &&
            y >= checkbox_y && y <= checkbox_y + g_panel.checkbox_size);
}

/**
 * Check if a point is inside the render button
 */
static bool is_point_in_render_button(int x, int y)
{
    int button_width = 100;
    int button_height = 30;
    int button_x = (g_panel.target_width - button_width) / 2;
    int button_y = g_panel.y + 50 + (35 * 5) + 20;
    
    return (x >= button_x && x <= button_x + button_width &&
            y >= button_y && y <= button_y + button_height);
}

/**
 * Handle mouse click on the UI panel
 * Returns true if the click was handled by the UI
 */
bool ui_panel_mouse_click(t_scene *scene, int x, int y)
{
    // Check if toggle button was clicked
    if (is_point_in_toggle_button(x, y)) {
        g_panel.visible = !g_panel.visible;
        
        // If closing, hide the panel image
        if (!g_panel.visible && g_panel.panel_img) {
            g_panel.panel_img->enabled = false;
        } else if (g_panel.panel_img) {
            g_panel.panel_img->enabled = true;
        }
        return true;
    }
    
    // If panel is not visible enough, ignore clicks on it
    if (g_panel.current_width < g_panel.target_width / 2)
        return false;
    
    // Check if click is within panel area
    if (x <= g_panel.current_width && y >= g_panel.y && y <= g_panel.y + g_panel.height) {
        int checkbox_x = g_panel.padding;
        int base_y = g_panel.y + 50;
        int spacing = 35;
        
        // Check shadows checkbox
        if (is_point_in_checkbox(x, y, checkbox_x, base_y)) {
            scene->graphic_settings.enable_hard_shadows = !scene->graphic_settings.enable_hard_shadows;
            return true;
        }
        
        // Check reflections checkbox
        if (is_point_in_checkbox(x, y, checkbox_x, base_y + spacing)) {
            scene->graphic_settings.enable_reflections = !scene->graphic_settings.enable_reflections;
            return true;
        }
        
        // Check specular checkbox
        if (is_point_in_checkbox(x, y, checkbox_x, base_y + spacing * 2)) {
            scene->graphic_settings.enable_specular = !scene->graphic_settings.enable_specular;
            return true;
        }
        
        // Check refraction checkbox
        if (is_point_in_checkbox(x, y, checkbox_x, base_y + spacing * 3)) {
            scene->graphic_settings.enable_refraction = !scene->graphic_settings.enable_refraction;
            return true;
        }
        
        // Check render button
        if (is_point_in_render_button(x, y)) {
            // Trigger re-render with current settings
            render_scene(scene);
            return true;
        }
        
        // Click was within panel but not on a control
        return true;
    }
    
    return false;
}

/**
 * Draw the UI panel
 * Returns true if the panel needs to be redrawn (animation in progress)
 */
bool draw_ui_panel(t_scene *scene)
{
    bool animation_active = update_panel_animation();
    
    // Clear the panel image if animation is active
    if (animation_active) {
        clear_panel_area(scene);
    }
    
    // Always draw the toggle button
    draw_toggle_button(scene);
    
    // Draw panel if it has any width
    if (g_panel.current_width > 0) {
        draw_panel_background(scene);
        draw_panel_content(scene);
    }
    
    // Show or hide panel based on visibility
    if (g_panel.panel_img) {
        g_panel.panel_img->enabled = (g_panel.current_width > 0);
    }
    
    return animation_active;
}

/**
 * Initialize the UI panel system
 */
void init_ui_panel(t_scene *scene)
{
    // Initialize panel state
    g_panel.visible = false;
    g_panel.current_width = 0;
    g_panel.text_initialized = false;
    
    // Create a dedicated image for the UI panel
    // Make it the size of the window to accommodate all UI elements
    g_panel.panel_img = mlx_new_image(scene->app.mlx, WIDTH, HEIGHT);
    if (!g_panel.panel_img) {
        fprintf(stderr, "Error: Failed to create UI panel image\n");
        return;
    }
    
    // Add the image to the window (initially invisible)
    mlx_image_to_window(scene->app.mlx, g_panel.panel_img, 0, 0);
    g_panel.panel_img->enabled = false;
    
    // Clear the panel image (make it transparent)
    clear_panel_area(scene);
}

/**
 * Clean up UI panel resources
 */
void cleanup_ui_panel(t_scene *scene)
{
    // Delete the panel image if it exists
    if (g_panel.panel_img) {
        mlx_delete_image(scene->app.mlx, g_panel.panel_img);
        g_panel.panel_img = NULL;
    }
    
    // Reset panel state
    g_panel.visible = false;
    g_panel.current_width = 0;
    g_panel.text_initialized = false;
} 