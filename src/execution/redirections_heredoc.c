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

static int	handle_signal_interrupt(t_shell *shell)
{
	if (g_signal_exit_status == SIGINT)
	{
		shell->state = 1;
		g_signal_exit_status = 0;
	}
	return (0);
}

static int	process_heredoc_line(char *line, char *delimiter, int pipe_fd)
{
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
	{
		free(line);
		return (0);
	}
	write(pipe_fd, line, ft_strlen(line));
	write(pipe_fd, "\n", 1);
	free(line);
	return (1);
}

static int	read_heredoc_line(char *delimiter, int pipe_fd, t_shell *shell)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		line = read_interactive_line();
	else
		line = read_non_interactive_line();
	if (!line || g_signal_exit_status == SIGINT)
		return (handle_signal_interrupt(shell));
	return (process_heredoc_line(line, delimiter, pipe_fd));
}

void	handle_heredoc(t_cmd *cmd, int i, t_shell *shell)
{
	int		pipe_fd[2];
	char	*delimiter;

	delimiter = cmd->redirs[i].file;
	if (!setup_heredoc_pipe(pipe_fd))
		return ;
	setup_heredoc_signals();
	while (read_heredoc_line(delimiter, pipe_fd[1], shell))
		continue ;
	close(pipe_fd[1]);
	if (shell->state == 1)
		handle_heredoc_error(pipe_fd, cmd, i, shell);
	else
		handle_heredoc_success(pipe_fd, cmd, i);
}
