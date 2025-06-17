/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 13:55:14 by abillote          #+#    #+#             */
/*   Updated: 2023/11/20 15:15:38 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*lsttemp;

	while (*lst != NULL)
	{
		lsttemp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = lsttemp;
	}
	free(*lst);
	*lst = NULL;
}
