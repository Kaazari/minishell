/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdjitte <zdjitte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:21:07 by zdjitte           #+#    #+#             */
/*   Updated: 2023/12/11 15:21:07 by zdjitte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t lenght)
{
	size_t	i;

	i = 0;
	if (lenght == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && s2[i] && i < lenght - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
