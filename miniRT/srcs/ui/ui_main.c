/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:41:12 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 00:41:13 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Main UI drawing function
*/
void	draw_ui(t_scene *scene)
{
	draw_toggle_button(scene);
	draw_ui_panel(scene);
	display_status(scene);
}
