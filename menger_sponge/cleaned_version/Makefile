# Detect OS
UNAME_S := $(shell uname -s)

# Set variables based on OS
ifeq ($(UNAME_S),Linux)
    MLX_PATH = minilibx-linux/
    MLX_LIB = libmlx.a
    MLX_FLAGS = -lXext -lX11 -lm -lz
endif
ifeq ($(UNAME_S),Darwin)
    MLX_PATH = minilibx_mms_20191025_beta/
    MLX_LIB = libmlx.dylib
    MLX_FLAGS = -framework OpenGL -framework AppKit
    INSTALL_NAME = install_name_tool -change libmlx.dylib @executable_path/libmlx.dylib
endif

# Common compilation settings
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3
INCLUDES = -I$(MLX_PATH) -Iincludes
LDFLAGS = -L$(MLX_PATH) -lmlx $(MLX_FLAGS)

# Directories
SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = obj

# Source files
SRC_FILES = main.c \
			events.c \
			init.c \
			string_utils.c \
			render.c \
			menger.c \
			colors.c \
			material.c \
			camera.c \
			render_complex_scenes.c \
			vector_utils/vec3_basic_op.c \
			vector_utils/vec3_length.c \
			vector_utils/vec3_product.c \
			parser/parser_utils.c \
			parser/parser_identifiers.c \
			parser/parser_identifiers2.c \
			parser/parser_camera.c \
			parser/parser_ambient.c \
			parser/parser_light.c \
			parser/parser_sphere.c \
			parser/parser_plane.c \
			parser/parser_cylinder.c \
			parser/parser_cone.c \
			parser/parser_file.c \
			parser/parser_line.c \
			parser/parser_types.c \
			utils/ft_atof.c \
			utils/ft_atoi.c	\
			utils/ft_isspace.c \
			utils/ft_split.c	\
			utils/ft_strtrim.c \
			utils/ft_calloc.c \
			utils/ft_bzero.c \
			utils/ft_memset.c	\
			utils/ft_strlcpy.c	\
			light/lights.c \
			shadows/shadows.c \
			objects/add_object.c \
			objects/cylinder/cylinder_intersect.c \
			objects/cylinder/cylinder_creation.c \
			objects/sphere/sphere_intersect.c \
			objects/sphere/sphere_creation.c \
			objects/plane/plane_intersect.c \
			objects/plane/plane_creation.c \
			objects/cube/cube_intersect.c \
			objects/cube/cube_creation.c \
			objects/triangle/triangle_intersect.c \
			objects/triangle/triangle_creation.c \
			objects/mesh/mesh_intersect.c \
			objects/mesh/mesh_creation.c \
			objects/cone/cone_creation.c \
			scenes/mesh_sphere_sphere.c \
			scenes/cube.c \
			scenes/triangle.c \
			scenes/spheres_sylinder_plane.c \
			scenes/sphere.c \
			scenes/sphere_cylinder.c \
			ui/user_interface.c\
			ui/control_panel.c

SOURCES = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJECTS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Output files
NAME = minirt

.PHONY: all clean fclean re obj_dir mlx

all: mlx obj_dir $(NAME)

# Build MLX
mlx:
	@$(MAKE) -C $(MLX_PATH)

# Create obj directory and subdirectories
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
	@mkdir -p $(OBJ_DIR)/scenes

# Build executable
$(NAME): $(OBJECTS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)
ifeq ($(UNAME_S),Darwin)
	@echo "Setting up dylib for macOS..."
	@cp $(MLX_PATH)$(MLX_LIB) .
	@$(INSTALL_NAME) $(NAME)
endif
	@echo "Build complete: $(NAME)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(MLX_PATH) clean

fclean: clean
	@echo "Removing executable and libraries..."
	@rm -f $(NAME)
ifeq ($(UNAME_S),Darwin)
	@rm -f $(MLX_LIB)
endif

re: fclean all
