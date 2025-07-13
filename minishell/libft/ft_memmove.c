/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdjitte <zdjitte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:22:46 by zdjitte           #+#    #+#             */
/*   Updated: 2023/12/11 15:22:47 by zdjitte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Copies len bytes from str src to str dst.
// The two str may overlap, copy is done in a non destructive manner.

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const char	*s;
	char		*d;
	size_t		i;

	s = (const char *)src;
	d = (char *)dst;
	if (!d && !s)
		return (NULL);
	i = 0;
	if (d < s)
	{
		while (len--)
		{
			d[i] = s[i];
			i++;
		}
	}
	else
		while (len--)
			d[len] = s[len];
	return ((void *)dst);
}
