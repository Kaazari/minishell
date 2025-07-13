/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdjitte <zdjitte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:24:12 by zdjitte           #+#    #+#             */
/*   Updated: 2023/12/11 15:24:13 by zdjitte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Can return 1 if c == 9,[...] 13 or 32 and 0 if not.
// But bc it's a non zero value we return, it's good too.

int	ft_isspace(int c)
{
	return (c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32);
}
