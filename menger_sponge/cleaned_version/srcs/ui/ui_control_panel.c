#include "../../includes/miniRT.h"

static const char *checkbox_labels[NUM_CHECKBOXES] =
{
    "Shadows",
    "Reflections",
    "Specular",
    "Refraction",
    "Status Message"
};

// Helper to map checkbox index → scene setting
static bool *get_checkbox_state(t_scene *scene, int idx)
{
    if (idx == 0) return &scene->graphic_settings.enable_hard_shadows;
    if (idx == 1) return &scene->graphic_settings.enable_reflections;
    if (idx == 2) return &scene->graphic_settings.enable_specular;
    if (idx == 3) return &scene->graphic_settings.enable_refraction;
    if (idx == 4) return &scene->graphic_settings.enable_status_message;
    return NULL;
}

static void clear_panel_image(t_panel *panel)
{
    if (!panel->panel_img) return;
    for (uint32_t y = 0; y < panel->panel_img->height; ++y)
        for (uint32_t x = 0; x < panel->panel_img->width; ++x)
            mlx_put_pixel(panel->panel_img, x, y, 0x00000000);
}

void init_toggle_button(t_scene *scene)
{
    t_toggle_button *tog;

    tog = &scene->ui.toggle;
    tog->offset_x = TOGGLE_BTN_OFFSET_X;
    tog->offset_y = TOGGLE_BTN_OFFSET_Y;
    tog->size = TOGGLE_BTN_SIZE;
    tog->toggle_img = mlx_new_image(scene->app.mlx, tog->size, tog->size);
    if (!tog->toggle_img) { fprintf(stderr, "Error: toggle btn img\n"); exit(1); }
    mlx_image_to_window(scene->app.mlx, tog->toggle_img, tog->offset_x, tog->offset_y);
    tog->toggle_img->enabled = true;
}

void init_ui_panel(t_scene *scene)
{
    t_panel *p = &scene->ui.panel;

    // Initialize all fixed fields
    p->visible         = false;
    p->current_width   = 0;
    p->target_width    = PANEL_WIDTH;
    p->height          = PANEL_HEIGHT;
    p->x               = PANEL_X;
    p->y               = PANEL_Y;
    p->animation_speed = 12;
    p->padding         = PANEL_PADDING;
    p->header_height   = PANEL_HEADER_HEIGHT;
    p->checkbox_size   = CHECKBOX_SIZE;
    p->checkbox_spacing= CHECKBOX_SPACING;
    p->text_count      = 0;
    p->status_text_img = NULL;
    memset(p->panel_text, 0, sizeof(p->panel_text));

    // Create the image
    p->panel_img = mlx_new_image(scene->app.mlx, p->target_width, p->height);
    if (!p->panel_img) { fprintf(stderr, "Error: panel img\n"); exit(1); }
    mlx_image_to_window(scene->app.mlx, p->panel_img, p->x, p->y);
    p->panel_img->enabled = true;
}

void cleanup_ui_panel(t_scene *scene)
{
    t_panel          *p  = &scene->ui.panel;
    t_toggle_button  *tog= &scene->ui.toggle;

    if (tog->toggle_img) {
        mlx_delete_image(scene->app.mlx, tog->toggle_img);
        tog->toggle_img = NULL;
    }
    if (p->panel_img) {
        mlx_delete_image(scene->app.mlx, p->panel_img);
        p->panel_img = NULL;
    }
    for (int i = 0; i < p->text_count; ++i) {
        if (p->panel_text[i]) {
            mlx_delete_image(scene->app.mlx, p->panel_text[i]);
            p->panel_text[i] = NULL;
        }
    }
    p->text_count = 0;
    if (p->status_text_img) {
        mlx_delete_image(scene->app.mlx, p->status_text_img);
        p->status_text_img = NULL;
    }
    p->visible       = false;
    p->current_width = 0;
}

static void draw_toggle_button(t_scene *scene)
{
    t_toggle_button *tog = &scene->ui.toggle;
    int s = tog->size;
    uint32_t btn = 0x333333FF, border = 0xFFFFFFFF, icon = 0xFFFFFFFF;

    for (int i = 0; i < s; ++i)
        for (int j = 0; j < s; ++j)
            mlx_put_pixel(tog->toggle_img, i, j, btn);
    for (int i = 0; i < s; ++i) {
        mlx_put_pixel(tog->toggle_img, i, 0, border);
        mlx_put_pixel(tog->toggle_img, i, s-1, border);
        mlx_put_pixel(tog->toggle_img, 0, i, border);
        mlx_put_pixel(tog->toggle_img, s-1, i, border);
    }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < s-10; ++j)
            mlx_put_pixel(tog->toggle_img, 5 + j, 8 + (i * 6), icon);
}

static void draw_checkbox(t_panel *p, mlx_image_t *img, int x, int y, bool checked)
{
    int size = p->checkbox_size;
    uint32_t bg = 0x222222FF, border = 0xFFFFFFFF, check = 0x00FF00FF;

    for (int i = 1; i < size-1; ++i)
        for (int j = 1; j < size-1; ++j)
            mlx_put_pixel(img, x + i, y + j, bg);
    for (int i = 0; i < size; ++i) {
        mlx_put_pixel(img, x + i, y, border);
        mlx_put_pixel(img, x + i, y + size - 1, border);
        mlx_put_pixel(img, x, y + i, border);
        mlx_put_pixel(img, x + size - 1, y + i, border);
    }
    if (checked)
        for (int j = 0; j < size-4; ++j)
        {
            mlx_put_pixel(img, x + 2 + j, y + 2 + j, check);
            mlx_put_pixel(img, x + size-3 - j, y + 2 + j, check);
        }
}

static void draw_panel_background(t_panel *p)
{
    if (p->current_width <= 0) return;
    uint32_t bg     = 0x33333399; // 60% transparent
    uint32_t header = 0x222222CC; // 80% opaque
    uint32_t border = 0xFFFFFFFF; // Opaque

    for (int x = 0; x < p->current_width; ++x)
        for (int y = 0; y < p->height; ++y)
            mlx_put_pixel(p->panel_img, x, y, bg);
    for (int x = 0; x < p->current_width; ++x)
        for (int y = 0; y < p->header_height; ++y)
            mlx_put_pixel(p->panel_img, x, y, header);
    for (int x = 0; x < p->current_width; ++x) {
        mlx_put_pixel(p->panel_img, x, 0,             border);
        mlx_put_pixel(p->panel_img, x, p->height-1,  border);
    }
    for (int y = 0; y < p->height; ++y) {
        if (p->current_width > 0)
            mlx_put_pixel(p->panel_img, 0, y, border);
        if (p->current_width > 1)
            mlx_put_pixel(p->panel_img, p->current_width - 1, y, border);
    }
}

static void delete_panel_text(t_scene *scene)
{
    t_panel *p = &scene->ui.panel;
    for (int i = 0; i < p->text_count; ++i) {
        if (p->panel_text[i]) {
            mlx_delete_image(scene->app.mlx, p->panel_text[i]);
            p->panel_text[i] = NULL;
        }
    }
    p->text_count = 0;
}

static void hide_panel_text(t_panel *p)
{
    for (int i = 0; i < p->text_count; ++i)
        if (p->panel_text[i])
            p->panel_text[i]->enabled = false;
}

static void show_panel_text(t_panel *p)
{
    for (int i = 0; i < p->text_count; ++i)
        if (p->panel_text[i])
            p->panel_text[i]->enabled = true;
}

void draw_panel_text(t_scene *scene)
{
    t_panel *p = &scene->ui.panel;
    delete_panel_text(scene);

    int idx = 0;

    // -- Title --
    int title_w = strlen("Settings") * 8; // 8 is average char width
    int title_x = (p->target_width - title_w) / 2;
    int title_y = p->header_height/2 - 8; // center vertically in header

    p->panel_text[idx] = mlx_put_string(scene->app.mlx, "Settings",
                                        p->x + title_x, p->y + title_y);
    p->text_offset_x[idx] = title_x;
    p->text_offset_y[idx] = title_y;
    idx++;

    // -- Checkbox Labels --
    int label_x = p->padding + p->checkbox_size + 12; // horizontal offset for labels
    int y_base  = p->header_height + p->padding;      // where the checkboxes/labels start (vertical offset from top)

    for (int i = 0; i < NUM_CHECKBOXES; ++i) {
        int label_y = y_base + i * p->checkbox_spacing; // vertical offset for this label
        p->panel_text[idx] = mlx_put_string(scene->app.mlx,
                                            checkbox_labels[i], p->x + label_x, p->y + label_y);
        p->text_offset_x[idx] = label_x;
        p->text_offset_y[idx] = label_y;
        idx++;
    }
    p->text_count = idx;
    show_panel_text(p);
}

static void draw_panel_content(t_scene *scene)
{
    t_panel *p = &scene->ui.panel;
    int x_cb   = p->padding;
    int y_base = p->header_height + p->padding;

    for (int i = 0; i < NUM_CHECKBOXES; ++i) {
        int y = y_base + i * p->checkbox_spacing;
        bool state = *get_checkbox_state(scene, i);
        draw_checkbox(p, p->panel_img, x_cb, y, state);
    }
}

static bool update_panel_animation(t_panel *p)
{
    if (p->visible)
    {
        if (p->current_width < p->target_width)
        {
            p->current_width += p->animation_speed;
            if (p->current_width > p->target_width)
                p->current_width = p->target_width;
            return true;
        }
    }
    else
    {
        if (p->current_width > 0)
        {
            p->current_width -= p->animation_speed;
            if (p->current_width < 0)
                p->current_width = 0;
            return true;
        }
    }
    return false;
}

bool draw_ui_panel(t_scene *scene)
{
    t_panel *p = &scene->ui.panel;
    bool    anim = update_panel_animation(p);

    clear_panel_image(p);
    if (p->current_width > 0)
    {
        draw_panel_background(p);
        draw_panel_content(scene);
    }
    if (p->current_width == p->target_width)
    {
        if (p->text_count == 0)
            draw_panel_text(scene);
        else
            show_panel_text(p);
    }
    else
    {
        hide_panel_text(p);
        if (p->current_width == 0 && p->text_count > 0)
            delete_panel_text(scene);
    }
    return anim;
}

static bool is_point_in_toggle_button(t_toggle_button *tog, int x, int y)
{
    return (x >= tog->offset_x && x <= tog->offset_x + tog->size &&
            y >= tog->offset_y && y <= tog->offset_y + tog->size);
}

static bool is_point_in_checkbox(t_panel *p, int x, int y, int idx)
{
    int cb_x = p->x + p->padding;
    int cb_y = p->y + p->header_height + p->padding + idx * p->checkbox_spacing;
    int s    = p->checkbox_size;
    return (x >= cb_x && x <= cb_x + s && y >= cb_y && y <= cb_y + s);
}

bool ui_panel_mouse_click(t_scene *scene, int x, int y)
{
    t_panel          *p   = &scene->ui.panel;
    t_toggle_button  *tog = &scene->ui.toggle;

    if (is_point_in_toggle_button(tog, x, y)) {
        p->visible = !p->visible;
        return true;
    }
    if (p->current_width < p->target_width / 2)
        return false;
    for (int i = 0; i < NUM_CHECKBOXES; ++i) {
        if (is_point_in_checkbox(p, x, y, i)) {
            bool *cb = get_checkbox_state(scene, i);
            if (cb) *cb = !(*cb);
            return true;
        }
    }
    return false;
}

void ui_animation_loop(void *param)
{
    t_scene *scene = (t_scene *)param;
    static double last = 0.0;
    double        now  = mlx_get_time();

    if (now - last > 0.016) {
        if (draw_ui_panel(scene)) {
            // still animating… no scene redraw needed
        }
        last = now;
    }
}

void draw_ui(t_scene *scene)
{
    draw_toggle_button(scene);
    draw_ui_panel(scene);
    display_status(scene);  // your existing status‐rendering function
}
