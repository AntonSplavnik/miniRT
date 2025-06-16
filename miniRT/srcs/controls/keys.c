#include "../../includes/miniRT.h"

void	key_callback(mlx_key_data_t keydata, void* param)
{
	t_scene	*scene;

	scene = (t_scene*)param;

	if(keydata.action != MLX_PRESS)
		return ;

	 // Handle key presses with if-else
    if (keydata.key == MLX_KEY_ESCAPE)
    {
        mlx_close_window(scene->app.mlx);
    }
    else if (keydata.key == MLX_KEY_C)
    {
    }
    else if (keydata.key == MLX_KEY_W)
    {
        printf("camera position before: \n x - %f\n y - %f\n z - %f\n", scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
        scene->camera.position = vec3_add(scene->camera.position, 
            vec3_scale(get_forward_vector(scene->camera.rotation), scene->camera.movement_speed));
        render_scene(scene);
        printf("camera position before: \n x - %f\n y - %f\n z - %f\n", scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
    }
    else if (keydata.key == MLX_KEY_S)
    {
        printf("S key is pressed\n");
        scene->camera.position = vec3_subtract(scene->camera.position, 
            vec3_scale(get_forward_vector(scene->camera.rotation), scene->camera.movement_speed));
        render_scene(scene);
    }
    else if (keydata.key == MLX_KEY_A)
    {
        scene->camera.position = vec3_subtract(scene->camera.position, 
            vec3_scale(get_right_vector(scene->camera.rotation), scene->camera.movement_speed));
        render_scene(scene);
    }
    else if (keydata.key == MLX_KEY_D)
    {
        scene->camera.position = vec3_add(scene->camera.position, 
            vec3_scale(get_right_vector(scene->camera.rotation), scene->camera.movement_speed));
        render_scene(scene);
    }
    else if (keydata.key == MLX_KEY_Q)
    {        scene->camera.position = vec3_subtract(scene->camera.position, 
            vec3_scale(get_up_vector(scene->camera.rotation), scene->camera.movement_speed));
        render_scene(scene);
    }
    else if (keydata.key == MLX_KEY_E)
    {
        scene->camera.position = vec3_add(scene->camera.position, 
            vec3_scale(get_up_vector(scene->camera.rotation), scene->camera.movement_speed));
        render_scene(scene);
    }
    else if (keydata.key == MLX_KEY_1)
    {
        scene->graphic_settings.ssaa_samples = 1;
        render_scene(scene);
    }
    else if (keydata.key == MLX_KEY_2)
    {
        scene->graphic_settings.ssaa_samples = 2;
        render_scene(scene);
    }
    else if (keydata.key == MLX_KEY_3)
    {
        scene->graphic_settings.ssaa_samples = 3;
        render_scene(scene);
    }
    else if (keydata.key == MLX_KEY_4)
    {
        scene->graphic_settings.ssaa_samples = 4;
        render_scene(scene);
    }
}

void	setup_key_hooks(t_scene	*scene)
{
	mlx_key_hook(scene->app.mlx, key_callback, scene);
}