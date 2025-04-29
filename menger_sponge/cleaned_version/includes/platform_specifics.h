/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   platform_specifics.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:10:14 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/29 10:41:09 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLATFORM_SPECIFIC_H
# define PLATFORM_SPECIFIC_H

/*
 * Platform-specific includes and definitions
 */

# ifdef __APPLE__
#  include "../minilibx_mms_20191025_beta/mlx.h"

// Define macOS key codes
#  define KEY_ESC 53
#  define KEY_UP 126
#  define KEY_DOWN 125
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_PLUS 24
#  define KEY_MINUS 27
#  define KEY_EQUAL 24
#  define KEY_R 15
#  define KEY_M 46
#  define KEY_1 18
#  define KEY_2 19
#  define KEY_3 20
#  define KEY_4 21
#  define KEY_5 23
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_BRACKET_LEFT 33
#  define KEY_BRACKET_RIGHT 30
#  define KEY_Q 12
#  define KEY_E 14
#  define KEY_B 11
#  define KEY_C 8
#  define KEY_F 3
#  define KEY_G 5
#  define KEY_H 4
#  define KEY_I 34
#  define KEY_J 38
#  define KEY_K 40
#  define KEY_L 37
#  define KEY_N 45
#  define KEY_O 31
#  define KEY_P 35
#  define KEY_T 17
#  define KEY_U 32
#  define KEY_V 9
#  define KEY_X 7
#  define KEY_Y 16
#  define KEY_Z 6

// Mouse button codes for macOS
#  define MOUSE_LEFT_BUTTON 1
#  define MOUSE_RIGHT_BUTTON 2
#  define MOUSE_SCROLL_UP 4
#  define MOUSE_SCROLL_DOWN 5

// Legacy compatibility macros
#  define KEY_bracketleft KEY_BRACKET_LEFT
#  define KEY_bracketright KEY_BRACKET_RIGHT
#  define KEY_r KEY_R

# else
// Linux/X11 includes and defines
#  include <X11/X.h>
#  include <X11/keysym.h>
#  include "../minilibx-linux/mlx.h"

// On Linux, we use X11's keysym.h, which already defines the key codes we need
// Here we define only what's not in keysym.h or needs to be remapped

// Mouse button codes already defined in X11 (Button1, Button2, etc.)

# endif

#endif
