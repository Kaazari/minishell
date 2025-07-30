/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
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

void	cleanup_pipex(t_shell *shell)
{
	if (shell && shell->pipex)
	{
		if (shell->pipex->pipe_fds)
			free(shell->pipex->pipe_fds);
		free(shell->pipex);
		shell->pipex = NULL;
	}
}

void	cleanup_cmd(t_shell *shell)
{
	if (shell && shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}
}

void	cleanup_envp(t_shell *shell)
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



void	cleanup_terminal_resources(void)
{
	struct termios	term;

	rl_clear_history();
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag |= ECHO | ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}
