/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Termine le processus enfant avec libération de mémoire
 *
 * Cette fonction :
 * - Libère le chemin de l'exécutable si fourni
 * - Termine le processus avec le statut spécifié
 *
 * @param path: Chemin de l'exécutable à libérer
 * @param status: Code de sortie
 */
void	child_exit(char *path, int status)
{
	if (path)
		free(path);
	exit(status);
}

/**
 * Gère la terminaison par signal du processus enfant
 *
 * Cette fonction :
 * - Extrait le numéro du signal du statut de terminaison
 * - Affiche un message approprié selon le signal
 * - Met à jour le statut de sortie du shell
 *
 * @param status: Status de terminaison
 * @param shell: Structure shell
 */
void	handle_signal_termination(int status, t_shell *shell)
{
	int	signal_num;

	signal_num = status & 0x7F;
	if (signal_num == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		shell->exit_status = 128 + signal_num;
	}
	else if (signal_num == SIGINT)
	{
		shell->exit_status = 128 + signal_num;
	}
	else if (signal_num == SIGTERM)
	{
		shell->exit_status = 128 + signal_num;
	}
	else
	{
		shell->exit_status = 128 + signal_num;
	}
}
