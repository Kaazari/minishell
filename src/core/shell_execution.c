/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_signal_exit_status;

/**
 * Exécute une commande shell avec gestion des redirections et du contexte
 *
 * Cette fonction est le point central d'exécution des commandes :
 * - Détermine si on est dans un contexte de pipe ou de commande simple
 * - Applique les redirections si nécessaire (pas dans un pipe)
 * - Exécute la commande (builtin ou externe)
 * - Restaure les redirections après exécution
 * - Gère la sortie des processus enfants dans les pipes
 *
 * Gestion du contexte :
 * - cmd_count > 1 indique un contexte de pipe
 * - Dans les pipes, les redirections sont gérées dans les processus enfants
 * - Dans les commandes simples, les redirections sont appliquées ici
 *
 * @param args: Arguments de la commande à exécuter
 * @param shell: Structure shell contenant l'état
 * @param cmd_count: Nombre de commandes dans le pipeline
 *
 * Note: Dans un contexte de pipe, cette fonction se termine avec exit()
 * pour permettre au processus parent de récupérer le statut de sortie
 */
void	execute_shell_command(char **args, t_shell *shell, int cmd_count)
{
	int	old_state;
	int	in_pipe_context;

	old_state = shell->state;
	shell->state = 3;
	in_pipe_context = (cmd_count > 1);
	handle_redirections_context(shell, in_pipe_context);
	execute_command_by_type(args, shell);
	restore_redirections_context(shell, in_pipe_context);
	if (in_pipe_context)
		clean_exit(shell, shell->exit_status);
	shell->state = old_state;
}
