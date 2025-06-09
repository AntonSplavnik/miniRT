

/*
// #include "../../includes/miniRT.h"
// 
//
// ----------- CONFIGURABLE MACROS ------------
#define PANEL_X             10
#define PANEL_Y             50
#define PANEL_WIDTH         260
#define PANEL_HEIGHT        400
#define PANEL_HEADER_HEIGHT 40
#define PANEL_PADDING       16
#define CHECKBOX_SIZE       16
#define CHECKBOX_SPACING    38
#define MAX_PANEL_TEXTS     16
#define NUM_CHECKBOXES      5

#define TOGGLE_BTN_OFFSET_X 10
#define TOGGLE_BTN_OFFSET_Y 10
#define TOGGLE_BTN_SIZE     30

//
// ----------- DATA STRUCTURES ------------
typedef struct s_panel {
    bool visible;
    int current_width;
    int target_width;
    int height;
    int x, y;
    int animation_speed;
    int padding;
    int header_height;
    int checkbox_size;
    int checkbox_spacing;
    mlx_image_t *panel_img;
    mlx_image_t *panel_text[MAX_PANEL_TEXTS];
    int text_count;
    mlx_image_t *status_text_img;
} t_panel;

typedef struct s_toggle_button {
    int offset_x, offset_y;
    int size;
    mlx_image_t *toggle_img;
} t_toggle_button;

typedef struct s_ui {
    t_panel panel;
    t_toggle_button toggle;
} t_ui;

//
// ----------- GLOBALS ------------
static t_panel g_panel = {
    .visible = false,
    .current_width = 0,
    .target_width = PANEL_WIDTH,
    .height = PANEL_HEIGHT,
    .x = PANEL_X,
    .y = PANEL_Y,
    .animation_speed = 12,
    .padding = PANEL_PADDING,
    .header_height = PANEL_HEADER_HEIGHT,
    .checkbox_size = CHECKBOX_SIZE,
    .checkbox_spacing = CHECKBOX_SPACING,
    .panel_img = NULL,
    .text_count = 0,
    .status_text_img = NULL
};

static t_toggle_button g_toggle = {
    .offset_x = TOGGLE_BTN_OFFSET_X,
    .offset_y = TOGGLE_BTN_OFFSET_Y,
    .size = TOGGLE_BTN_SIZE,
    .toggle_img = NULL
};

//
// ----------- CHECKBOX LABELS ------------
static const char *checkbox_labels[NUM_CHECKBOXES] = {
    "Shadows", "Reflections", "Specular", "Refraction", "Status Message"
};

//
// ----------- HELPERS ------------
static bool *get_checkbox_state(t_scene *scene, int idx) {
    // Change according to your struct
    if (idx == 0) return &scene->graphic_settings.enable_hard_shadows;
    if (idx == 1) return &scene->graphic_settings.enable_reflections;
    if (idx == 2) return &scene->graphic_settings.enable_specular;
    if (idx == 3) return &scene->graphic_settings.enable_refraction;
    if (idx == 4) return &scene->graphic_settings.enable_status_message;
    return NULL;
}

static void clear_panel_image(void) {
    if (!g_panel.panel_img) return;
    for (uint32_t y = 0; y < g_panel.panel_img->height; ++y)
        for (uint32_t x = 0; x < g_panel.panel_img->width; ++x)
            mlx_put_pixel(g_panel.panel_img, x, y, 0x00000000);
}

//
// ----------- PANEL AND TOGGLE INITIALIZATION ------------
void init_toggle_button(t_scene *scene) {
    g_toggle.toggle_img = mlx_new_image(scene->app.mlx, g_toggle.size, g_toggle.size);
    if (!g_toggle.toggle_img) { fprintf(stderr, "Error: toggle btn img\n"); exit(1);}
    mlx_image_to_window(scene->app.mlx, g_toggle.toggle_img, g_toggle.offset_x, g_toggle.offset_y);
    g_toggle.toggle_img->enabled = true;
}

void init_ui_panel(t_scene *scene) {
    g_panel.current_width = 0;
    g_panel.text_count = 0;
    g_panel.panel_img = mlx_new_image(scene->app.mlx, PANEL_WIDTH, PANEL_HEIGHT);
    if (!g_panel.panel_img) { fprintf(stderr, "Error: panel img\n"); exit(1);}
    mlx_image_to_window(scene->app.mlx, g_panel.panel_img, g_panel.x, g_panel.y);
    g_panel.panel_img->enabled = true;
}

void cleanup_ui_panel(t_scene *scene) {
    if(g_toggle.toggle_img) {
        mlx_delete_image(scene->app.mlx, g_toggle.toggle_img);
        g_toggle.toggle_img = NULL;
    }
    if (g_panel.panel_img) {
        mlx_delete_image(scene->app.mlx, g_panel.panel_img);
        g_panel.panel_img = NULL;
    }
    for (int i = 0; i < g_panel.text_count; ++i)
        if (g_panel.panel_text[i]) {
            mlx_delete_image(scene->app.mlx, g_panel.panel_text[i]);
            g_panel.panel_text[i] = NULL;
        }
    g_panel.text_count = 0;
    if (g_panel.status_text_img) {
        mlx_delete_image(scene->app.mlx, g_panel.status_text_img);
        g_panel.status_text_img = NULL;
    }
    g_panel.visible = false;
    g_panel.current_width = 0;
}

//
// ----------- DRAWING UI ELEMENTS ------------
static void draw_toggle_button(void) {
    int s = g_toggle.size;
    uint32_t btn = 0x333333FF, border = 0xFFFFFFFF, icon = 0xFFFFFFFF;
    for (int i = 0; i < s; ++i)
        for (int j = 0; j < s; ++j)
            mlx_put_pixel(g_toggle.toggle_img, i, j, btn);
    for (int i = 0; i < s; ++i) {
        mlx_put_pixel(g_toggle.toggle_img, i, 0, border);
        mlx_put_pixel(g_toggle.toggle_img, i, s-1, border);
        mlx_put_pixel(g_toggle.toggle_img, 0, i, border);
        mlx_put_pixel(g_toggle.toggle_img, s-1, i, border);
    }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < s-10; ++j)
            mlx_put_pixel(g_toggle.toggle_img, 5+j, 8+(i*6), icon);
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
    uint32_t bg     = 0x33333399; // 60% transparent
    uint32_t header = 0x222222CC; // 80% opaque header
    uint32_t border = 0xFFFFFFFF; // Opaque border

    for (int x = 0; x < g_panel.current_width; ++x)
        for (int y = 0; y < g_panel.height; ++y)
            mlx_put_pixel(g_panel.panel_img, x, y, bg);
    for (int x = 0; x < g_panel.current_width; ++x)
        for (int y = 0; y < g_panel.header_height; ++y)
            mlx_put_pixel(g_panel.panel_img, x, y, header);
    for (int x = 0; x < g_panel.current_width; ++x) {
        mlx_put_pixel(g_panel.panel_img, x, 0, border);
        mlx_put_pixel(g_panel.panel_img, x, g_panel.height - 1, border);
    }
    for (int y = 0; y < g_panel.height; ++y) {
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
        if (g_panel.panel_text[i]) g_panel.panel_text[i]->enabled = false;
}
static void show_panel_text(void) {
    for (int i = 0; i < g_panel.text_count; ++i)
        if (g_panel.panel_text[i]) g_panel.panel_text[i]->enabled = true;
}
static void draw_panel_text(t_scene *scene) {
    delete_panel_text(scene->app.mlx);
    int idx = 0;
    int title_x = (g_panel.target_width - (int)strlen("Settings") * 8) / 2;
    g_panel.panel_text[idx++] = mlx_put_string(scene->app.mlx, "Settings", g_panel.x + title_x, g_panel.y + g_panel.header_height/2 - 8);
    int x_label = g_panel.x + g_panel.padding + g_panel.checkbox_size + 12;
    int y_base = g_panel.y + g_panel.header_height + g_panel.padding;
    for (int i = 0; i < NUM_CHECKBOXES; ++i) {
        int y = y_base + i * g_panel.checkbox_spacing;
        g_panel.panel_text[idx++] = mlx_put_string(scene->app.mlx, checkbox_labels[i], x_label, y);
    }
    g_panel.text_count = idx;
    show_panel_text();
}
static void draw_panel_content(t_scene *scene) {
    int x_cb = g_panel.padding, y_base = g_panel.header_height + g_panel.padding;
    for (int i = 0; i < NUM_CHECKBOXES; ++i) {
        int y = y_base + i * g_panel.checkbox_spacing;
        draw_checkbox(g_panel.panel_img, x_cb, y, *get_checkbox_state(scene, i));
    }
}

//
// ----------- ANIMATION AND MAIN PANEL DRAW ------------
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
    if (g_panel.current_width > 0) {
        draw_panel_background();
        draw_panel_content(scene);
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

//
// ----------- INPUT HANDLING ------------
static bool is_point_in_toggle_button(int x, int y)
{
    int tx = g_toggle.offset_x;
    int ty = g_toggle.offset_y;
    int s = g_toggle.size;
    return (x >= tx && x <= tx + s && y >= ty && y <= ty + s);
}
static bool is_point_in_checkbox(int x, int y, int idx) {
    int cb_x = g_panel.x + g_panel.padding;
    int cb_y = g_panel.y + g_panel.header_height + g_panel.padding + idx * g_panel.checkbox_spacing;
    int s = g_panel.checkbox_size;
    return (x >= cb_x && x <= cb_x + s && y >= cb_y && y <= cb_y + s);
}
bool ui_panel_mouse_click(t_scene *scene, int x, int y) {
    if (is_point_in_toggle_button(x, y)) {
        g_panel.visible = !g_panel.visible;
        return true;
    }
    if (g_panel.current_width < g_panel.target_width / 2)
        return false;
    for (int i = 0; i < NUM_CHECKBOXES; ++i) {
        if (is_point_in_checkbox(x, y, i)) {
            bool *cb = get_checkbox_state(scene, i);
            if (cb) *cb = !(*cb);
            return true;
        }
    }
    return false;
}

//
// ----------- LOOP HOOK FOR ANIMATION ------------
void ui_animation_loop(void *param) {
    t_scene *scene = (t_scene *)param;
    static double last_update_time = 0;
    double current_time = mlx_get_time();
    if (current_time - last_update_time > 0.016) {
        if (draw_ui_panel(scene)) {
            // Animation still running, no need to rerender scene
        }
        last_update_time = current_time;
    }
}

//
// ----------- STATUS MESSAGE ------------
// void draw_status_message(t_scene *scene) {
//     if (g_panel.status_text_img) {
//         mlx_delete_image(scene->app.mlx, g_panel.status_text_img);
//         g_panel.status_text_img = NULL;
//     }
//     if (scene->graphic_settings.enable_status_message) {
//         const char *msg = "Hello, this is a status!";
//         int msg_x = (WIDTH - (int)strlen(msg) * 8) / 2;
//         int msg_y = 8;
//         g_panel.status_text_img = mlx_put_string(scene->app.mlx, msg, msg_x, msg_y);
//         if (g_panel.status_text_img)
//             g_panel.status_text_img->enabled = true;
//     }
// }

//
// ----------- MAIN UI RENDER CALL ------------
void draw_ui(t_scene *scene)
{
    draw_toggle_button();
    draw_ui_panel(scene);
    display_status(scene);
    // draw_status_message(scene);
}
 */

