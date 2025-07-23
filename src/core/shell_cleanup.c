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
#include <termios.h>
#include <unistd.h>

static void	cleanup_pipex(t_shell *shell)
{
	if (shell && shell->pipex)
	{
		if (shell->pipex->pipe_fds)
			free(shell->pipex->pipe_fds);
		free(shell->pipex);
		shell->pipex = NULL;
	}
}

static void	cleanup_cmd(t_shell *shell)
{
	if (shell && shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}
}

static void	cleanup_envp(t_shell *shell)
{
	char	**tmp;

	if (shell && shell->local_envp)
	{
		tmp = shell->local_envp;
		while (*tmp)
			free(*tmp++);
		free(shell->local_envp);
		shell->local_envp = NULL;
	}
	if (shell && shell->envp)
	{
		tmp = shell->envp;
		while (*tmp)
			free(*tmp++);
		free(shell->envp);
		shell->envp = NULL;
	}
}

static void	cleanup_temp_structures(t_shell *shell)
{
	// This function ensures all temporary structures are freed
	// even if they weren't properly stored in the shell
	if (shell && shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}

	// Free current commands if they exist
	if (shell && shell->current_commands)
	{
		free_partial_cmds(shell->current_commands, shell->current_cmd_count);
		shell->current_commands = NULL;
		shell->current_cmd_count = 0;
	}
}

static void	force_cleanup(t_shell *shell)
{
	if (shell && shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}

	if (shell && shell->envp)
	{
		char **tmp = shell->envp;
		while (*tmp)
			free(*tmp++);
		free(shell->envp);
		shell->envp = NULL;
	}

	if (shell && shell->local_envp)
	{
		char **tmp = shell->local_envp;
		while (*tmp)
			free(*tmp++);
		free(shell->local_envp);
		shell->local_envp = NULL;
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

static void	cleanup_terminal_resources(void)
{
	// Force cleanup of terminal resources
	rl_clear_history();
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();

	// Reset terminal state
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag |= ECHO | ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
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

	// Better cleanup of readline and terminal
	clear_history();
	cleanup_terminal_resources();

	exit(status);
}
