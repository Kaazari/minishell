/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	force_cleanup_env(t_shell *shell)
{
	char	**tmp;

	if (shell && shell->envp)
	{
		tmp = shell->envp;
		while (*tmp)
			free(*tmp++);
		free(shell->envp);
		shell->envp = NULL;
	}
	if (shell && shell->local_envp)
	{
		tmp = shell->local_envp;
		while (*tmp)
			free(*tmp++);
		free(shell->local_envp);
		shell->local_envp = NULL;
	}
}

static void	force_cleanup_structures(t_shell *shell)
{
	if (shell && shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}
	if (shell && shell->pipex)
	{
		if (shell->pipex->pipe_fds)
			free(shell->pipex->pipe_fds);
		free(shell->pipex);
		shell->pipex = NULL;
	}
	cleanup_temp_structures(shell);
}

static void	force_cleanup(t_shell *shell)
{
	force_cleanup_structures(shell);
	force_cleanup_env(shell);
}

void	clean_exit(t_shell *shell, int status)
{
	if (shell)
	{
		cleanup_pipex(shell);
		cleanup_cmd(shell);
		cleanup_envp(shell);
		force_cleanup(shell);
	}
	clear_history();
	cleanup_terminal_resources();
	exit(status);
}
