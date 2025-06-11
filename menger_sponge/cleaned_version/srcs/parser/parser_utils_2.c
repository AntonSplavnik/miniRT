/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:40:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 12:02:53 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/**
 * Utility function to clamp a value between min and max
 *
 * @param value The value to clamp
 * @param min The minimum allowed value
 * @param max The maximum allowed value
 * @return The clamped value
 */
double	ft_clamp(double value, double min, double max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

/**
 * Find the substring in a string (like standard strstr)
 *
 * @param haystack The string to search in
 * @param needle The substring to find
 * @return Pointer to the start of the substring, or NULL if not found
 */
char *ft_strstr(const char *haystack, const char *needle)
{
    size_t i;
    size_t j;

    if (!needle[0])
        return ((char *)haystack);
    i = 0;
    while (haystack[i])
    {
        j =
        0;
        while (haystack[i + j] && needle[j] && haystack[i + j] == needle[j])
            j++;
        if (!needle[j])
            return ((char *)&haystack[i]);
        i++;
    }
    return (NULL);
}

/**
 * Concatenate strings with size checking (like standard strlcat)
 *
 * @param dst Destination string
 * @param src Source string to append
 * @param size Size of the destination buffer
 * @return Total length of the string they tried to create
 */
size_t ft_strlcat(char *dst, const char *src, size_t size)
{
    size_t i;
    size_t dst_len;
    size_t src_len;

    i = 0;
    dst_len = 0;
    src_len = 0;

    while (src[src_len])
        src_len++;

    while (dst[dst_len] && dst_len < size)
        dst_len++;

    if (dst_len == size)
        return (dst_len + src_len);

    while (src[i] && (dst_len + i + 1) < size)
    {
        dst[dst_len + i] = src[i];
        i++;
    }
    dst[dst_len + i] = '\0';

    return (dst_len + src_len);
}
