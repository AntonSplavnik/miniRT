/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:42:46 by abillote          #+#    #+#             */
/*   Updated: 2023/11/20 15:16:15 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*lsttemp;

	if (!lst)
		return (NULL);
	lsttemp = lst;
	while (lsttemp->next != NULL)
	{
		lsttemp = lsttemp->next;
	}
	return (lsttemp);
}
