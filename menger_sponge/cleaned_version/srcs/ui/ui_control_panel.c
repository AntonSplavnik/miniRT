#include "../../includes/miniRT.h"
#include "../../MLX42/include/MLX42/MLX42.h"
#include <string.h>

#define PANEL_WIDTH     250
#define PANEL_HEIGHT    400
#define PANEL_X         0
#define PANEL_Y         50
#define MAX_PANEL_TEXTS 16

typedef struct s_ui_panel {
    bool visible;
    int current_width;
    int target_width;
    int height;
    int x, y;
    int toggle_button_size;
    int animation_speed;
    int padding;
    int checkbox_size;
    mlx_image_t *panel_img;
    mlx_image_t *panel_text[MAX_PANEL_TEXTS];
    int text_count;
    mlx_image_t *status_text_img;
} t_ui_panel;

static t_ui_panel g_panel = {
    .visible = false,
    .current_width = 0,
    .target_width = PANEL_WIDTH,
    .height = PANEL_HEIGHT,
    .x = PANEL_X,
    .y = PANEL_Y,
    .toggle_button_size = 30,
    .animation_speed = 10,
    .padding = 15,
    .checkbox_size = 15,
    .panel_img = NULL,
    .text_count = 0,
    .status_text_img = NULL
};

static void clear_panel_image(void) {
    if (!g_panel.panel_img) return;
    for (uint32_t y = 0; y < g_panel.panel_img->height; ++y)
        for (uint32_t x = 0; x < g_panel.panel_img->width; ++x)
            mlx_put_pixel(g_panel.panel_img, x, y, 0x00000000);
}

static void draw_toggle_button(void) {
    int s = g_panel.toggle_button_size, x = 10, y = 10;
    uint32_t btn = 0x333333FF, border = 0xFFFFFFFF, icon = 0xFFFFFFFF;
    for (int i = 0; i < s; ++i)
        for (int j = 0; j < s; ++j)
            mlx_put_pixel(g_panel.panel_img, x+i, y+j, btn);
    for (int i = 0; i < s; ++i) {
        mlx_put_pixel(g_panel.panel_img, x+i, y, border);
        mlx_put_pixel(g_panel.panel_img, x+i, y+s-1, border);
        mlx_put_pixel(g_panel.panel_img, x, y+i, border);
        mlx_put_pixel(g_panel.panel_img, x+s-1, y+i, border);
    }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < s-10; ++j)
            mlx_put_pixel(g_panel.panel_img, x+5+j, y+8+(i*6), icon);
}

static void draw_checkbox(mlx_image_t *img, int x, int y, bool checked) {
    int size = g_panel.checkbox_size;
    uint32_t bg = 0x222222FF, border = 0xFFFFFFFF, check = 0x00FF00FF;
    for (int i = 1; i < size-1; ++i)
        for (int j = 1; j < size-1; ++j)
            mlx_put_pixel(img, x+i, y+j, bg);
    for (int i = 0; i < size; ++i) {
        mlx_put_pixel(img, x+i, y, border);
        mlx_put_pixel(img, x+i, y+size-1, border);
        mlx_put_pixel(img, x, y+i, border);
        mlx_put_pixel(img, x+size-1, y+i, border);
    }
    if (checked)
        for (int j = 0; j < size-4; ++j) {
            mlx_put_pixel(img, x+2+j, y+2+j, check);
            mlx_put_pixel(img, x+size-3-j, y+2+j, check);
        }
}

static void draw_panel_background(void) {
    if (g_panel.current_width <= 0) return;
    uint32_t bg = 0x333333EE, header = 0x222222EE, border = 0xFFFFFFFF;
    for (int x = 0; x < g_panel.current_width; ++x)
        for (int y = g_panel.y; y < g_panel.y + g_panel.height; ++y)
            mlx_put_pixel(g_panel.panel_img, x, y, bg);
    for (int x = 0; x < g_panel.current_width; ++x)
        for (int y = g_panel.y; y < g_panel.y + 40; ++y)
            mlx_put_pixel(g_panel.panel_img, x, y, header);
    for (int x = 0; x < g_panel.current_width; ++x) {
        mlx_put_pixel(g_panel.panel_img, x, g_panel.y, border);
        mlx_put_pixel(g_panel.panel_img, x, g_panel.y + g_panel.height - 1, border);
    }
    for (int y = g_panel.y; y < g_panel.y + g_panel.height; ++y) {
        if (g_panel.current_width > 0)
            mlx_put_pixel(g_panel.panel_img, 0, y, border);
        if (g_panel.current_width > 1)
            mlx_put_pixel(g_panel.panel_img, g_panel.current_width - 1, y, border);
    }
}

static void delete_panel_text(mlx_t *mlx) {
    for (int i = 0; i < g_panel.text_count; ++i)
        if (g_panel.panel_text[i]) {
            mlx_delete_image(mlx, g_panel.panel_text[i]);
            g_panel.panel_text[i] = NULL;
        }
    g_panel.text_count = 0;
}
static void hide_panel_text(void) {
    for (int i = 0; i < g_panel.text_count; ++i)
        if (g_panel.panel_text[i]) 
            g_panel.panel_text[i]->enabled = false;
}
static void show_panel_text(void) {
    for (int i = 0; i < g_panel.text_count; ++i)
        if (g_panel.panel_text[i]) 
            g_panel.panel_text[i]->enabled = true;
}

// Only called if fully open and text not created
static void draw_panel_text(t_scene *scene) {
    delete_panel_text(scene->app.mlx);

    int idx = 0;
    int title_x = (g_panel.target_width - (int)strlen("Settings") * 8) / 2;
    g_panel.panel_text[idx++] = mlx_put_string(scene->app.mlx, "Settings", title_x, g_panel.y + 15);

    int label_x = g_panel.padding + g_panel.checkbox_size + 10;
    int base_y = g_panel.y + 50, spacing = 35, text_y_offset = (g_panel.checkbox_size - 16) / 2;
    g_panel.panel_text[idx++] = mlx_put_string(scene->app.mlx, "Shadows", label_x, base_y + text_y_offset);
    g_panel.panel_text[idx++] = mlx_put_string(scene->app.mlx, "Reflections", label_x, base_y + spacing + text_y_offset);
    g_panel.panel_text[idx++] = mlx_put_string(scene->app.mlx, "Specular", label_x, base_y + spacing * 2 + text_y_offset);
    g_panel.panel_text[idx++] = mlx_put_string(scene->app.mlx, "Refraction", label_x, base_y + spacing * 3 + text_y_offset);
    g_panel.panel_text[idx++] = mlx_put_string(scene->app.mlx, "Status Message", label_x, base_y + spacing * 4 + text_y_offset);

    g_panel.text_count = idx;
    show_panel_text();
}

static void draw_panel_content(t_scene *scene) {
    int checkbox_x = g_panel.padding, base_y = g_panel.y + 50, spacing = 35;
    if (g_panel.current_width < g_panel.target_width / 2) return;
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y, scene->graphic_settings.enable_hard_shadows);
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y + spacing, scene->graphic_settings.enable_reflections);
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y + spacing*2, scene->graphic_settings.enable_specular);
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y + spacing*3, scene->graphic_settings.enable_refraction);
    draw_checkbox(g_panel.panel_img, checkbox_x, base_y + spacing*4, scene->graphic_settings.enable_status_message);
}

static bool update_panel_animation(void) {
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

bool draw_ui_panel(t_scene *scene) {
    bool animating = update_panel_animation();
    clear_panel_image();
    draw_toggle_button();

    if (g_panel.current_width > 0) {
        draw_panel_background();
        draw_panel_content(scene);
        g_panel.panel_img->enabled = true;
    } else {
        g_panel.panel_img->enabled = false;
    }

    if (g_panel.current_width == g_panel.target_width) {
        if (g_panel.text_count == 0)
            draw_panel_text(scene);
        else
            show_panel_text();
    } else {
        hide_panel_text();
        if (g_panel.current_width == 0 && g_panel.text_count > 0)
            delete_panel_text(scene->app.mlx);
    }
    return animating;
}

static bool is_point_in_toggle_button(int x, int y) {
    return (x >= 10 && x <= 10 + g_panel.toggle_button_size &&
            y >= 10 && y <= 10 + g_panel.toggle_button_size);
}
static bool is_point_in_checkbox(int x, int y, int checkbox_x, int checkbox_y) {
    return (x >= checkbox_x && x <= checkbox_x + g_panel.checkbox_size &&
            y >= checkbox_y && y <= checkbox_y + g_panel.checkbox_size);
}

bool ui_panel_mouse_click(t_scene *scene, int x, int y) {
    int checkbox_x = g_panel.padding, base_y = g_panel.y + 50, spacing = 35;
    if (is_point_in_toggle_button(x, y)) {
        g_panel.visible = !g_panel.visible;
        return true;
    }
    if (g_panel.current_width < g_panel.target_width / 2)
        return false;

    if (x <= g_panel.current_width && y >= g_panel.y && y <= g_panel.y + g_panel.height) {
        if (is_point_in_checkbox(x, y, checkbox_x, base_y))
            scene->graphic_settings.enable_hard_shadows = !scene->graphic_settings.enable_hard_shadows;
        else if (is_point_in_checkbox(x, y, checkbox_x, base_y + spacing))
            scene->graphic_settings.enable_reflections = !scene->graphic_settings.enable_reflections;
        else if (is_point_in_checkbox(x, y, checkbox_x, base_y + spacing*2))
            scene->graphic_settings.enable_specular = !scene->graphic_settings.enable_specular;
        else if (is_point_in_checkbox(x, y, checkbox_x, base_y + spacing*3))
            scene->graphic_settings.enable_refraction = !scene->graphic_settings.enable_refraction;
        else if (is_point_in_checkbox(x, y, checkbox_x, base_y + spacing*4))
            scene->graphic_settings.enable_status_message = !scene->graphic_settings.enable_status_message;
        else
            return false;
        return true;
    }
    return false;
}

void init_ui_panel(t_scene *scene) {
    g_panel.visible = false;
    g_panel.current_width = 0;
    g_panel.text_count = 0;
    g_panel.panel_img = mlx_new_image(scene->app.mlx, WIDTH, HEIGHT);
    if (!g_panel.panel_img) {
        fprintf(stderr, "Error: Failed to create UI panel image\n");
        exit(1);
    }
    mlx_image_to_window(scene->app.mlx, g_panel.panel_img, 0, 0);
    g_panel.panel_img->enabled = false;
    clear_panel_image();
}

void cleanup_ui_panel(t_scene *scene) {
    if (g_panel.panel_img) {
        mlx_delete_image(scene->app.mlx, g_panel.panel_img);
        g_panel.panel_img = NULL;
    }
    delete_panel_text(scene->app.mlx);
    if (g_panel.status_text_img) {
        mlx_delete_image(scene->app.mlx, g_panel.status_text_img);
        g_panel.status_text_img = NULL;
    }
    g_panel.visible = false;
    g_panel.current_width = 0;
}

// --- STATUS MESSAGE AT TOP ---

// void draw_status_message(t_scene *scene)
// {
//     // Remove previous status image if any
//     if (g_panel.status_text_img) {
//         mlx_delete_image(scene->app.mlx, g_panel.status_text_img);
//         g_panel.status_text_img = NULL;
//     }
//     if (scene->graphic_settings.enable_status_message) {
//         // Center at top
//         const char *msg = "Hello, this is a status!";
//         int msg_x = (WIDTH - (int)strlen(msg) * 8) / 2;
//         int msg_y = 8;
//         g_panel.status_text_img = mlx_put_string(scene->app.mlx, msg, msg_x, msg_y);
//         if (g_panel.status_text_img)
//             g_panel.status_text_img->enabled = true;
//     }
// }

// Call this in your main rendering/UI loop, **after** draw_ui_panel(scene)
void draw_ui(t_scene *scene)
{
    draw_ui_panel(scene);           // Draws the panel, with all its controls and panel text
    display_status(scene);          // Shows status at top if enabled
}
