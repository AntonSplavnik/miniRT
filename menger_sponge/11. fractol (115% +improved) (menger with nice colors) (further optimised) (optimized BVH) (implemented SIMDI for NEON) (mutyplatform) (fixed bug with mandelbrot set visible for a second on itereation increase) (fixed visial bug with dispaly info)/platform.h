/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   platform.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:10:14 by asplavni          #+#    #+#             */
/*   Updated: 2024/10/14 14:16:25 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLATFORM_H
# define PLATFORM_H

# ifdef __APPLE__
#  include "fractol_mac.h"
# else
#  include "fractol.h"
# endif

#endif 