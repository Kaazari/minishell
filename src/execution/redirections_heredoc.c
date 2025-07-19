/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	setup_heredoc_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

static int	read_heredoc_line(char *delimiter, int pipe_fd, t_shell *shell)
{
	char	*line;

	line = readline("> ");
	if (!line || g_signal == SIGINT)
	{
		if (line)
			free(line);
		if (g_signal == SIGINT)
		{
			shell->state = 1;
			g_signal = 0;
		}
		return (0);
	}
	if (strcmp(line, delimiter) == 0)
	{
		free(line);
		return (0);
	}
	write(pipe_fd, line, strlen(line));
	write(pipe_fd, "\n", 1);
	free(line);
	return (1);
}

void	handle_heredoc(t_cmd *cmd, int i, t_shell *shell)
{
	int		pipe_fd[2];
	char	*delimiter;
	int		stdin_backup;

	delimiter = cmd->redirs[i].file;
	if (!setup_heredoc_pipe(pipe_fd))
		return ;
	stdin_backup = dup(STDIN_FILENO);
	setup_heredoc_signals();
	while (read_heredoc_line(delimiter, pipe_fd[1], shell))
		continue ;
	close(pipe_fd[1]);
	if (shell->state == 1)
	{
		close(pipe_fd[0]);
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		restore_main_signals();
		shell->state = 0;
		return ;
	}
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	close(stdin_backup);
	restore_main_signals();
}
