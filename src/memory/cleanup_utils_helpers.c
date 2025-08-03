/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Nettoie l'environnement local
 *
 * @param shell: Structure shell contenant l'environnement à nettoyer
 */
void	cleanup_local_envp(t_shell *shell)
{
	if (shell && shell->local_envp)
	{
		free_envp(shell->local_envp);
		shell->local_envp = NULL;
	}
}

/**
 * Nettoie l'environnement système
 *
 * @param shell: Structure shell contenant l'environnement à nettoyer
 */
void	cleanup_system_envp(t_shell *shell)
{
	if (shell && shell->envp)
	{
		free_envp(shell->envp);
		shell->envp = NULL;
	}
}

/**
 * Nettoie les chemins de travail
 *
 * @param shell: Structure shell contenant les chemins à nettoyer
 */
void	cleanup_work_paths(t_shell *shell)
{
	if (shell && shell->pwd)
	{
		free(shell->pwd);
		shell->pwd = NULL;
	}
	if (shell && shell->oldpwd)
	{
		free(shell->oldpwd);
		shell->oldpwd = NULL;
	}
}
