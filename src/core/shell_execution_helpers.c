/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execution_helpers.c                          :+:      :+:    :+:   */
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
 * Gère la commande exit avec ses arguments
 *
 * @param args: Arguments de la commande
 * @param shell: Structure shell
 */
void	handle_exit_command(char **args, t_shell *shell)
{
	int	result;

	result = builtin_exit(args, shell);
	if (result >= 1000)
	{
		shell->exit_status = result - 1000;
		clean_exit(shell, shell->exit_status);
	}
}

/**
 * Gère les redirections dans le contexte de commande simple
 *
 * @param shell: Structure shell
 * @param in_pipe_context: Indique si on est dans un contexte de pipe
 */
void	handle_redirections_context(t_shell *shell, int in_pipe_context)
{
	if (shell->cmd && (!in_pipe_context))
		handle_redirections(shell->cmd, shell);
}

/**
 * Restaure les redirections dans le contexte de commande simple
 *
 * @param shell: Structure shell
 * @param in_pipe_context: Indique si on est dans un contexte de pipe
 */
void	restore_redirections_context(t_shell *shell, int in_pipe_context)
{
	if (shell->cmd && (!in_pipe_context))
		restore_redirections(shell->cmd);
}

/**
 * Exécute une commande builtin ou externe selon le nom de la commande
 *
 * Cette fonction :
 * - Identifie le type de commande (builtin ou externe)
 * - Exécute la commande builtin appropriée
 * (cd, pwd, echo, env, export, unset, exit)
 * - Délègue l'exécution des commandes externes à execute_external_commands
 * - Gère la commande ":" (commande vide qui retourne toujours 0)
 *
 * Gestion des signaux :
 * - Met g_signal_exit_status à 999 pendant l'exécution
 * pour éviter l'affichage du prompt
 * - Remet g_signal_exit_status à 0 après exécution
 *
 * @param args: Arguments de la commande à exécuter
 * @param shell: Structure shell contenant l'état
 *
 * Note: La commande ":" est une commande spéciale qui ne fait rien et retourne 0
 */
void	execute_builtin_command(char **args, t_shell *shell)
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
 * Exécute la commande selon son type
 *
 * @param args: Arguments de la commande
 * @param shell: Structure shell
 */
void	execute_command_by_type(char **args, t_shell *shell)
{
	if (args && args[0] && ft_strlen(args[0]) > 0)
		execute_builtin_command(args, shell);
	else if (args && args[0] && ft_strlen(args[0]) == 0)
	{
		printf("minishell: : command not found\n");
		shell->exit_status = 127;
	}
}
