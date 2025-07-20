/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:59:33 by asplavni          #+#    #+#             */
/*   Updated: 2024/12/19 16:26:45 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdlib.h>
# include <stdint.h>
# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *format, ...);

/* Characters */
int		ft_putchar(char c);
int		ft_putstr(char *s);
size_t	ft_strlen(const char *s);

/* Digits */
int		ft_putdigit(long long n, int base);
int		ft_hexa(long long n, int base, char UP_or_low);
int		ft_pointer(long long n, int base);

#endif
