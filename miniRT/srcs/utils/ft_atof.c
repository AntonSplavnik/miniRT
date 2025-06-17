/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 09:52:24 by abillote          #+#    #+#             */
/*   Updated: 2025/06/09 14:20:12 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Convert string to float - handle integer part
static void	ft_atof_integer_part(const char *str, int *i, double *result)
{
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		*result = (*result * 10.0) + (str[*i] - '0');
		(*i)++;
	}
}

// Convert string to float - handle decimal part
static void	ft_atof_decimal_part(const char *str, int *i, double *result)
{
	double	decimal;
	double	factor;

	if (str[*i] == '.')
	{
		(*i)++;
		decimal = 0.0;
		factor = 0.1;
		while (str[*i] >= '0' && str[*i] <= '9')
		{
			decimal += (str[*i] - '0') * factor;
			factor *= 0.1;
			(*i)++;
		}
		*result += decimal;
	}
}

// Convert string to float
double	ft_atof(const char *str)
{
	int		i;
	int		sign;
	double	result;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	sign = 1;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	result = 0.0;
	ft_atof_integer_part(str, &i, &result);
	ft_atof_decimal_part(str, &i, &result);
	return (sign * result);
}
