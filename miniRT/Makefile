# Detect OS
UNAME_S := $(shell uname -s)

# MLX42 configuration
MLX42_PATH = MLX42
MLX42_INCLUDE = $(MLX42_PATH)/include
MLX42_BUILD = $(MLX42_PATH)/build
MLX42_REPO = https://github.com/codam-coding-college/MLX42.git

# Libft configuration
LIBFT_PATH = libft
LIBFT_LIB = $(LIBFT_PATH)/libft.a

# Compiler & Flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -pthread
INCLUDES = -I$(MLX42_INCLUDE) -Iincludes -I$(LIBFT_PATH)

# OS-specific linker flags
ifeq ($(UNAME_S),Linux)
	MLX42_FLAGS = -ldl -lglfw -pthread -lm
endif
ifeq ($(UNAME_S),Darwin)
	MLX42_FLAGS = -framework Cocoa -framework OpenGL -framework IOKit $(shell pkg-config --libs glfw3)
endif

LDFLAGS = -L$(MLX42_BUILD) -lmlx42 $(MLX42_FLAGS) -L$(LIBFT_PATH) -lft

# Directories
SRC_DIR = srcs
OBJ_DIR = obj
INC_DIR = includes

# All source files (preserve subdirectory structure)
SRC_FILES = main.c \
	cleanup.c \
	close_handler.c \
	init.c \
	init_hooks.c \
	camera/camera.c \
	camera/camera_vectors.c \
	controls/keys.c \
	controls/key_handlers.c \
	controls/mouse.c \
	controls/mouse_cursor.c \
	controls/mouse_handlers.c \
	objects/add_object.c \
	objects/cone/cone_intersect.c \
	objects/cone/cone_intersect_equation.c \
	objects/cone/cone_normal.c \
	objects/cube/cube_intersect.c \
	objects/cube/cube_intersect_utils.c \
	objects/cube/cube_normal.c \
	objects/cylinder/cylinder_intersect.c \
	objects/cylinder/cylinder_intersect_caps.c \
	objects/cylinder/cylinder_normal.c \
	objects/mesh/mesh_intersect.c \
	objects/mesh/mesh_transform.c \
	objects/mesh/mesh_linear_intersect.c \
	objects/mesh/mesh_bary_intersect.c \
	objects/plane/plane_intersect.c \
	objects/sphere/sphere_intersect.c \
	objects/triangle/triangle_intersect_bary.c \
	objects/triangle/triangle_intersect.c \
	parser/parser_ambient.c \
	parser/parser_camera.c \
	parser/parser_cone.c \
	parser/parser_cube.c \
	parser/parser_cylinder.c \
	parser/parser_file.c \
	parser/parser_identifiers.c \
	parser/parser_identifiers2.c \
	parser/parser_light.c \
	parser/parser_line.c \
	parser/parser_material.c \
	parser/parser_material_properties.c \
	parser/parser_material_helpers.c \
	parser/parser_mesh.c \
	parser/parser_mesh_faces.c \
	parser/parser_mesh_face_processing.c \
	parser/parser_mesh_loader.c \
	parser/parser_mesh_loader_helpers.c \
	parser/parser_mesh_triangle_helpers.c \
	parser/parser_plane.c \
	parser/parser_sphere.c \
	parser/parser_triangle.c \
	parser/parser_data.c \
	parser/parser_utils.c \
	parser/parser_utils_2.c \
	parser/parser_bumpmap.c \
	parser/parser_bumpmap_ppm_file.c \
	parser/parser_bumpmap_ppm_header.c \
	parser/parser_texture.c \
	rendering/checker/checker.c \
	rendering/checker/checker_utils.c \
	rendering/checker/cubic_map.c \
	rendering/color/color_conversion.c \
	rendering/color/tone_mapping.c \
	rendering/color/hdr_processing.c \
	rendering/color/lighting.c \
	rendering/color/color_calculations.c \
	rendering/color/texture_sampling.c \
	rendering/color/surface_colors.c \
	rendering/intersections/compute_intersection.c \
	rendering/intersections/compute_normals.c \
	rendering/intersections/compute_ray_intersection.c \
	rendering/intersections/compute_ray_intersection_2.c \
	rendering/intersections/find_intersaction.c \
	rendering/BVH/calculate_object_aabb.c \
	rendering/BVH/calculate_object_aabb_2.c \
	rendering/BVH/calculate_object_aabb_3.c \
	rendering/BVH/calculate_object_aabb_4.c \
	rendering/BVH/calculate_object_aabb_5.c \
	rendering/BVH/calculate_object_aabb_6.c \
	rendering/BVH/build_scene_bvh.c \
	rendering/BVH/build_scene_bvh_2.c \
	rendering/BVH/build_scene_bvh_3.c \
	rendering/BVH/build_scene_bvh_helpers.c \
	rendering/BVH/build_scene_bvh_utils.c \
	rendering/BVH/scene_ray_intersect_bvh.c \
	rendering/BVH/scene_ray_intersect_bvh_2.c \
	rendering/BVH/scene_ray_intersect_bvh_3.c \
	rendering/BVH/scene_ray_intersect_bvh_4.c \
	rendering/BVH/scene_ray_intersect_bvh_5.c \
	rendering/BVH/ray_intersect_aabb.c \
	rendering/BVH/ray_intersect_aabb_2.c \
	rendering/BVH/mesh_bvh.c \
	rendering/BVH/mesh_bvh_utils.c \
	rendering/BVH/mesh_partition_utils.c \
	rendering/BVH/mesh_bvh_split.c \
	rendering/BVH/mesh_bvh_helpers.c \
	rendering/BVH/bvh_intersect/bvh_intersect_batch.c \
	rendering/BVH/bvh_intersect/bvh_intersect_leaves.c \
	rendering/BVH/bvh_intersect/bvh_intersect_utils.c \
	rendering/BVH/bvh_intersect/bvh_helpers.c \
	rendering/BVH/bvh_intersect/mesh_bvh_intersect_helpers.c \
	rendering/BVH/bvh_intersect/mesh_bvh_intersect.c \
	rendering/BVH/bvh_intersect/mesh_bvh_traversal_helpers.c \
	rendering/lighting/compute_light.c \
	rendering/rays/compute_rays.c \
	rendering/reflection/reflection.c \
	rendering/refraction/refraction.c \
	rendering/renderer/render_shared.c \
	rendering/renderer/render_single.c \
	rendering/renderer/render_utils.c \
	rendering/trace_ray/trace_rays.c \
	rendering/trace_ray/trace_ray_helpers.c \
	rendering/trace_ray/background_utils.c \
	rendering/fresnel/fresnel_calculations.c \
	rendering/reflection/reflection_operations.c \
	rendering/refraction/refraction_operations.c \
	rendering/materials/material_processors.c \
	rendering/texture_uv_mapping/cube_uv.c \
	rendering/texture_uv_mapping/cylinder_uv.c \
	rendering/texture_uv_mapping/plane_uv.c \
	rendering/texture_uv_mapping/sphere_uv.c \
	rendering/texture_uv_mapping/texture_uv_main.c \
	rendering/bumpmap/bumpmap.c \
	rendering/bumpmap/bump_normal.c \
	rendering/bumpmap/cylinder_cube_tangents.c \
	rendering/bumpmap/tangent_vectors.c \
	rendering/shadows/shadows.c \
	ui/ui_control_panel.c \
	ui/ui_panel_utils.c \
	ui/ui_drawing.c \
	ui/ui_checkbox_drawing.c \
	ui/ui_panel_drawing.c \
	ui/ui_text_management.c \
	ui/ui_text_drawing.c \
	ui/ui_interaction.c \
	ui/ui_panel_helpers.c\
	ui/ui_main.c \
	ui/status_messages.c \
	ui/status_formatting.c \
	utils/ft_atof.c \
	utils/ft_strstr.c \
	utils/ft_isspace.c \
	utils/ft_split_line.c \
	utils/ft_split_line_utils.c \
	utils/ft_strtrim_whitespace.c \
	utils/ft_strcmp.c \
	vector_utils/vec3_basic_op.c \
	vector_utils/vec3_length.c \
	vector_utils/vec3_product.c \
	vector_utils/vec3_barycentric.c

# Bonus-specific source files (replace main.c and single-threaded rendering with bonus files)
BONUS_SRC_FILES = $(filter-out main.c rendering/renderer/render_single.c, $(SRC_FILES)) \
	main_bonus.c \
	parser/parser_light_bonus.c \
	parser/parser_line_bonus.c \
	parser/parser_file_bonus.c \
	rendering/renderer/render_multithreaded.c

# File generation
SOURCES = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
BONUS_SOURCES = $(addprefix $(SRC_DIR)/, $(BONUS_SRC_FILES))
OBJECTS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
BONUS_OBJECTS = $(addprefix $(OBJ_DIR)/, $(BONUS_SRC_FILES:.c=.o))

# Output
NAME = minirt
BONUS_NAME = minirt_bonus

# Build tracking files
BUILD_MARKER = .build_marker
BONUS_MARKER = .bonus_marker

# Tell Make where to find .c files
vpath %.c $(SRC_DIR)

.PHONY: all clean fclean re obj_dir mlx mlx_check mlx_clean libft print bonus bonus_re

# Default rule
all: mlx libft obj_dir $(NAME)

# Bonus rule
bonus: mlx libft obj_dir $(BONUS_NAME)

# Rebuild rules
re: fclean all

bonus_re: fclean bonus

# Executable linking - regular
$(NAME): $(OBJECTS) $(LIBFT_LIB)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)
ifeq ($(UNAME_S),Darwin)
	@cp $(MLX42_BUILD)/libmlx42.a ./
endif
	@touch $(BUILD_MARKER)
	@rm -f $(BONUS_MARKER)
	@echo "Build complete: $(NAME)"

# Executable linking - bonus
$(BONUS_NAME): $(BONUS_OBJECTS) $(LIBFT_LIB)
	@echo "Linking $(BONUS_NAME)..."
	@$(CC) $(CFLAGS) $(BONUS_OBJECTS) $(LDFLAGS) -o $(BONUS_NAME)
ifeq ($(UNAME_S),Darwin)
	@cp $(MLX42_BUILD)/libmlx42.a ./
endif
	@touch $(BONUS_MARKER)
	@rm -f $(BUILD_MARKER)
	@echo "Build complete: $(BONUS_NAME)"

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create object directory tree
obj_dir:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/vector_utils
	@mkdir -p $(OBJ_DIR)/parser
	@mkdir -p $(OBJ_DIR)/light
	@mkdir -p $(OBJ_DIR)/shadows
	@mkdir -p $(OBJ_DIR)/objects
	@mkdir -p $(OBJ_DIR)/objects/cylinder
	@mkdir -p $(OBJ_DIR)/objects/sphere
	@mkdir -p $(OBJ_DIR)/objects/plane
	@mkdir -p $(OBJ_DIR)/objects/cube
	@mkdir -p $(OBJ_DIR)/objects/triangle
	@mkdir -p $(OBJ_DIR)/objects/mesh
	@mkdir -p $(OBJ_DIR)/objects/cone
	@mkdir -p $(OBJ_DIR)/rendering
	@mkdir -p $(OBJ_DIR)/rendering/color
	@mkdir -p $(OBJ_DIR)/rendering/trace_ray
	@mkdir -p $(OBJ_DIR)/rendering/fresnel
	@mkdir -p $(OBJ_DIR)/rendering/reflection
	@mkdir -p $(OBJ_DIR)/rendering/refraction
	@mkdir -p $(OBJ_DIR)/rendering/materials
	@mkdir -p $(OBJ_DIR)/rendering/lighting
	@mkdir -p $(OBJ_DIR)/rendering/rays
	@mkdir -p $(OBJ_DIR)/rendering/BVH
	@mkdir -p $(OBJ_DIR)/rendering/checker
	@mkdir -p $(OBJ_DIR)/rendering/texture_uv_mapping
	@mkdir -p $(OBJ_DIR)/rendering/bumpmap
	@mkdir -p $(OBJ_DIR)/ui
	@mkdir -p $(OBJ_DIR)/controls
	@mkdir -p $(OBJ_DIR)/camera
	@mkdir -p $(OBJ_DIR)/utils

# Build libft
libft:
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_PATH)

# Clone MLX42 if missing
mlx_check:
	@if [ ! -d "$(MLX42_PATH)" ]; then \
		echo "MLX42 not found. Cloning from GitHub..."; \
		git clone $(MLX42_REPO) $(MLX42_PATH); \
	fi

# Build MLX42
mlx: mlx_check
	@mkdir -p $(MLX42_BUILD)
	@cd $(MLX42_BUILD) && cmake .. -DDEBUG=1
	@$(MAKE) -C $(MLX42_BUILD)

# Clean only MLX42 build artifacts
mlx_clean:
	@if [ -d "$(MLX42_BUILD)" ]; then \
		$(MAKE) -C $(MLX42_BUILD) clean; \
		echo "MLX42 build cleaned."; \
	fi

# Remove entire MLX42 folder (library and build)
mlx_remove:
	@echo "Removing MLX42 library and build directory..."
	@rm -rf $(MLX42_PATH)

# Clean build files (works for both regular and bonus)
clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@if [ -d "$(MLX42_BUILD)" ]; then rm -rf $(MLX42_BUILD); fi
	@$(MAKE) -C $(LIBFT_PATH) clean

# Full clean (works for both regular and bonus)
fclean: clean
	@echo "Removing binaries and static libs..."
	@rm -f $(NAME) $(BONUS_NAME)
	@rm -f $(BUILD_MARKER) $(BONUS_MARKER)
ifeq ($(UNAME_S),Darwin)
	@rm -f libmlx42.a
endif
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@echo "Clean complete."

# Debug rule
print:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "BONUS_SOURCES: $(BONUS_SOURCES)"
	@echo "BONUS_OBJECTS: $(BONUS_OBJECTS)"
