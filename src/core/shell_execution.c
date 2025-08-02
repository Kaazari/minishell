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

/**
 * Gère l'exécution de la commande exit avec gestion spéciale
 *
 * Cette fonction :
 * - Appelle builtin_exit pour traiter la commande exit
 * - Si le résultat est >= 1000, cela indique une demande de sortie
 * - Met à jour shell->exit_status et termine le processus
 *
 * @param args: Arguments de la commande exit
 * @param shell: Structure shell contenant l'état
 *
 * Note: Le code de retour >= 1000 est utilisé pour distinguer une demande
 * de sortie d'un simple statut de retour
 */
static void	handle_exit_command(char **args, t_shell *shell)
{
	int	result;

	result = builtin_exit(args, shell);
	if (result >= 1000)
	{
		shell->exit_status = result - 1000;
		exit(shell->exit_status);
	}
}

/**
 * Exécute une commande builtin ou externe selon le nom de la commande
 *
 * Cette fonction :
 * - Identifie le type de commande (builtin ou externe)
 * - Exécute la commande builtin appropriée (cd, pwd, echo, env, export, unset, exit)
 * - Délègue l'exécution des commandes externes à execute_external_commands
 * - Gère la commande ":" (commande vide qui retourne toujours 0)
 *
 * Gestion des signaux :
 * - Met g_signal_exit_status à 999 pendant l'exécution pour éviter l'affichage du prompt
 * - Remet g_signal_exit_status à 0 après exécution
 *
 * @param args: Arguments de la commande à exécuter
 * @param shell: Structure shell contenant l'état
 *
 * Note: La commande ":" est une commande spéciale qui ne fait rien et retourne 0
 */
static void	execute_builtin_command(char **args, t_shell *shell)
{
	g_signal_exit_status = 999;
	if (ft_strncmp(args[0], "cd", 3) == 0)
		shell->exit_status = builtin_cd(args, shell);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		shell->exit_status = builtin_pwd(shell);
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		shell->exit_status = builtin_echo(args, shell);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		shell->exit_status = builtin_env(shell);
	else if (ft_strncmp(args[0], "export", 7) == 0)
		shell->exit_status = builtin_export(args, shell);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		shell->exit_status = builtin_unset(args, shell);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		handle_exit_command(args, shell);
	else if (ft_strncmp(args[0], ":", 2) == 0)
		shell->exit_status = 0;
	else
		execute_external_commands(args, shell->envp, shell);
	g_signal_exit_status = 0;
}

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

	if (shell->cmd && (!in_pipe_context))
		handle_redirections(shell->cmd, shell);

	if (args && args[0] && ft_strlen(args[0]) > 0)
		execute_builtin_command(args, shell);
	else if (args && args[0] && ft_strlen(args[0]) == 0)
	{
		printf("minishell: : command not found\n");
		shell->exit_status = 127;
	}

	if (shell->cmd && (!in_pipe_context))
		restore_redirections(shell->cmd);

	if (in_pipe_context)
	{
		exit(shell->exit_status);
	}

	shell->state = old_state;
}
