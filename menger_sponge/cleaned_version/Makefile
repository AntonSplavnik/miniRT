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
            lights.c \
            material.c \
            objects_creation.c \
            cylinder_intersect.c \
            vector_utils/vec3_basic_op.c \
            vector_utils/vec3_length.c \
            vector_utils/vec3_product.c \
            render_complex_scenes.c \
            shadows.c \
            sphere_intersect.c \
            camera.c \
            plane_intersect.c

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
