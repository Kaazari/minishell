/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_empty_input(char *input)
{
	int	i;

	i = 0;
	if (strlen(input) == 0)
		return (1);
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n'
			&& input[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}
