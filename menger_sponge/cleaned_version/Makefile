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
	material.c \
	string_utils.c \
	camera/camera.c \
	controls/keys.c \
	controls/mouse.c \
	light/lights.c \
	objects/add_object.c \
	objects/cone/cone_creation.c \
	objects/cone/cone_intersect.c \
	objects/cube/cube_creation.c \
	objects/cube/cube_intersect.c \
	objects/cylinder/cylinder_creation.c \
	objects/cylinder/cylinder_intersect.c \
	objects/mesh/mesh_creation.c \
	objects/mesh/mesh_intersect.c \
	objects/plane/plane_creation.c \
	objects/plane/plane_intersect.c \
	objects/sphere/sphere_creation.c \
	objects/sphere/sphere_intersect.c \
	objects/triangle/triangle_creation.c \
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
	parser/parser_mesh.c \
	parser/parser_plane.c \
	parser/parser_sphere.c \
	parser/parser_triangle.c \
	parser/parser_types.c \
	parser/parser_utils.c \
	parser/parser_utils_2.c \
	rendering/checker.c \
	rendering/colors.c \
	rendering/compute_intersections.c \
	rendering/compute_light.c \
	rendering/compute_rays.c \
	rendering/reflection.c \
	rendering/render.c \
	rendering/render_utils.c \
	rendering/trace_rays.c \
	ui/control_panel.c \
	ui/status_messages.c \
	ui/user_interface.c \
	utils/ft_atof.c \
	utils/ft_atoi.c \
	utils/ft_bzero.c \
	utils/ft_calloc.c \
	utils/ft_isspace.c \
	utils/ft_memset.c \
	utils/ft_split.c \
	utils/ft_strlcpy.c \
	utils/ft_strtrim.c \
	vector_utils/vec3_basic_op.c \
	vector_utils/vec3_length.c \
	vector_utils/vec3_product.c

# File generation
SOURCES = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJECTS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Output
NAME = minirt

# Tell Make where to find .c files
vpath %.c $(SRC_DIR)

.PHONY: all clean fclean re obj_dir mlx mlx_check mlx_clean libft print

# Default rule
all: mlx libft obj_dir $(NAME)

# Executable linking
$(NAME): $(OBJECTS) $(LIBFT_LIB)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)
ifeq ($(UNAME_S),Darwin)
	@cp $(MLX42_BUILD)/libmlx42.a ./
endif
	@echo "Build complete: $(NAME)"

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

# Clean build files
clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@if [ -d "$(MLX42_BUILD)" ]; then rm -rf $(MLX42_BUILD); fi
	@$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	@echo "Removing binary and static libs..."
	@rm -f $(NAME)
ifeq ($(UNAME_S),Darwin)
	@rm -f libmlx42.a
endif
	@$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

# Debug rule
print:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
