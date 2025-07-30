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



static void	force_cleanup(t_shell *shell)
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
	if (shell->current_commands)
	{
		free_partial_cmds(shell->current_commands, shell->current_cmd_count);
		shell->current_commands = NULL;
		shell->current_cmd_count = 0;
	}
	cleanup_envp(shell);
}

void	clean_exit(t_shell *shell, int status)
{
	if (shell)
	{
		force_cleanup(shell);
	}
	clear_history();
	cleanup_terminal_resources();
	exit(status);
}
