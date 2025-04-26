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
INCLUDES = -I$(MLX_PATH)
LDFLAGS = -L$(MLX_PATH) -lmlx $(MLX_FLAGS)

# Source files
SOURCES = main.c events.c init.c math_utils.c render.c string_utils.c \
          handle_pixel.c thread_render.c render_fractal_progressive.c menger.c

# Output files
NAME = fractol
OBJ_DIR = obj

# Object files (in a simpler format to handle spaces)
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/events.o $(OBJ_DIR)/init.o \
          $(OBJ_DIR)/math_utils.o $(OBJ_DIR)/render.o $(OBJ_DIR)/string_utils.o \
          $(OBJ_DIR)/handle_pixel.o $(OBJ_DIR)/thread_render.o \
          $(OBJ_DIR)/render_fractal_progressive.o $(OBJ_DIR)/menger.o

.PHONY: all clean fclean re obj_dir mlx

all: mlx obj_dir $(NAME)

# Build MLX
mlx:
	@$(MAKE) -C $(MLX_PATH)

# Create obj directory
obj_dir:
	@mkdir -p $(OBJ_DIR)

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

# Object file compilation rules - one for each file to avoid dealing with complex patterns
$(OBJ_DIR)/main.o: main.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/events.o: events.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/init.o: init.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/math_utils.o: math_utils.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/render.o: render.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/string_utils.o: string_utils.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/handle_pixel.o: handle_pixel.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/thread_render.o: thread_render.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/render_fractal_progressive.o: render_fractal_progressive.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/menger.o: menger.c
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