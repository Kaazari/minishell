/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdjitte <zdjitte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:21:57 by zdjitte           #+#    #+#             */
/*   Updated: 2023/12/11 15:21:58 by zdjitte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int chr)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char) chr)
			break ;
		i++;
	}
	if (s[i] == (char) chr)
		return ((char *) s + i);
	return (NULL);
}
