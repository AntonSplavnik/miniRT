/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:44:20 by abillote          #+#    #+#             */
/*   Updated: 2025/04/28 17:44:35 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Vector utilities
t_vec3  vec3_create(double x, double y, double z);
t_vec3  vec3_negate(t_vec3 v);
t_vec3  vec3_add(t_vec3 u, t_vec3 v);
t_vec3  vec3_subtract(t_vec3 u, t_vec3 v);
t_vec3  vec3_multiply(t_vec3 u, t_vec3 v);
t_vec3  vec3_scale(t_vec3 v, double t);
t_vec3  vec3_divide(t_vec3 v, double t);
double  vec3_dot(t_vec3 u, t_vec3 v);
t_vec3  vec3_cross(t_vec3 u, t_vec3 v);
double  vec3_length_squared(t_vec3 v);
double  vec3_length(t_vec3 v);
t_vec3  vec3_normalize(t_vec3 v);
