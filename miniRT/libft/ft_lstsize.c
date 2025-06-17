/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:42:46 by abillote          #+#    #+#             */
/*   Updated: 2023/11/20 12:40:10 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		n;
	t_list	*lsttemp;

	n = 0;
	lsttemp = lst;
	while (lsttemp != NULL)
	{
		n++;
		lsttemp = lsttemp->next;
	}
	return (n);
}
