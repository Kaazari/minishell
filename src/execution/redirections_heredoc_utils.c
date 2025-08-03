/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Vérifie les signaux et EOF lors de la lecture
 *
 * @param bytes_read: Nombre d'octets lus
 * @return 1 si signal/EOF détecté, 0 sinon
 */
int	check_signal_or_eof(int bytes_read)
{
	if (g_signal_exit_status == SIGINT)
		return (1);
	if (bytes_read <= 0)
	{
		if (g_signal_exit_status == SIGINT)
			return (1);
		if (bytes_read == 0)
			return (1);
		return (1);
	}
	return (0);
}

/**
 * Lit une ligne depuis stdin (mode non-interactif)
 *
 * @return Ligne lue ou NULL en cas d'erreur
 */
char	*read_line_from_stdin(void)
{
	char	buffer[1000];
	int		i;
	int		bytes_read;
	char	c;

	i = 0;
	while (i < 999)
	{
		if (g_signal_exit_status == SIGINT)
			return (NULL);
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (check_signal_or_eof(bytes_read))
			return (NULL);
		if (c == '\n')
		{
			buffer[i] = c;
			buffer[i + 1] = '\0';
			return (ft_strdup(buffer));
		}
		buffer[i] = c;
		i++;
	}
	buffer[i] = '\0';
	return (ft_strdup(buffer));
}
