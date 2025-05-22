/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 19:44:13 by asplavni          #+#    #+#             */
/*   Updated: 2024/10/15 15:34:34 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

double	map(double unscaled_num, t_bounds bounds)
{
	return (((bounds.new_max - bounds.new_min) * (unscaled_num - bounds.old_min)
			/ (bounds.old_max - bounds.old_min)) + bounds.new_min);
}

t_complex	sum_complex(t_complex z1, t_complex z2)
{
	t_complex	result;

	result.x = z1.x + z2.x;
	result.y = z1.y + z2.y;
	return (result);
}

t_complex	square_complex(t_complex z)
{
	t_complex	result;

	result.x = (z.x * z.x) - (z.y * z.y);
	result.y = 2 * z.x * z.y;
	return (result);
}

int	is_valid_double(const char *str)
{
	int	i;
	int	has_decimal_point;
	int	has_digits;

	i = 0;
	has_decimal_point = 0;
	has_digits = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] == '.')
		{
			if (has_decimal_point)
				return (0);
			has_decimal_point = 1;
		}
		else if (!isdigit(str[i]))
			return (0);
		else
			has_digits = 1;
		i++;
	}
	return (has_digits);
}
