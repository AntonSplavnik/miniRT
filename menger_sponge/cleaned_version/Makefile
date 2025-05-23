# Detect OS
UNAME_S := $(shell uname -s)

# MLX42 configuration
MLX42_PATH = MLX42
MLX42_INCLUDE = $(MLX42_PATH)/include
MLX42_BUILD = $(MLX42_PATH)/build

# OS-specific settings
ifeq ($(UNAME_S),Linux)
    MLX42_FLAGS = -ldl -lglfw -pthread -lm
endif
ifeq ($(UNAME_S),Darwin)
    MLX42_FLAGS = -framework Cocoa -framework OpenGL -framework IOKit -lglfw
endif

# Common compilation settings
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -pthread
INCLUDES = -I$(MLX42_INCLUDE) -Iincludes
LDFLAGS = -L$(MLX42_BUILD) -lmlx42 $(MLX42_FLAGS)

# Directories
SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = obj

# Source files
SRC_FILES = main.c \
			init.c \
			string_utils.c \
			colors.c \
			material.c \
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
			parser/parser_triangle.c \
			parser/parser_cube.c \
			parser/parser_mesh.c \
			parser/parser_material.c	\
			parser/parser_utils_2.c		\
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
			objects/cone/cone_intersect.c \
			close_handler.c \
			rendering/render.c \
			rendering/menger.c \
			rendering/compute_light.c	\
			rendering/compute_rays.c	\
			rendering/compute_intersections.c	\
			rendering/render_utils.c	\
			rendering/checkerboard.c	\
			camera/camera.c \
			ui/user_interface.c\
			ui/control_panel.c\
			ui/status_messages.c\
			controls/keys.c\
			controls/mouse.c

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
	@mkdir -p $(OBJ_DIR)/ui
	@mkdir -p $(OBJ_DIR)/controls
	@mkdir -p $(OBJ_DIR)/camera
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/objects/cone

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
