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
		write(STDOUT_FILENO, "\n", 1);
		shell->state = 1;
		g_signal_exit_status = 0;
	}
	return (0);
}

static int	process_heredoc_line(char *line, char *delimiter, int pipe_fd)
{
	char	*clean_line;
	int		len;

	if (!line)
		return (1);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		clean_line = ft_substr(line, 0, len - 1);
	else
		clean_line = ft_strdup(line);
	if (!clean_line)
		return (free(line), 1);
	if (ft_strncmp(clean_line, delimiter, ft_strlen(delimiter)) == 0)
	{
		free(line);
		free(clean_line);
		return (1);
	}
	write(pipe_fd, line, ft_strlen(line));
	free(line);
	free(clean_line);
	return (0);
}

static int	read_heredoc_line(char *delimiter, int pipe_fd, t_shell *shell)
{
	char	*line;

	if (g_signal_exit_status == SIGINT)
		return (handle_signal_interrupt(shell));
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("> ", STDERR_FILENO);
	line = read_non_interactive_line();
	if (!line)
	{
		if (g_signal_exit_status == SIGINT)
			return (0);
		shell->state = 1;
		return (0);
	}
	if (process_heredoc_line(line, delimiter, pipe_fd))
		return (0);
	return (1);
}

void	handle_heredoc(t_cmd *cmd, int i, t_shell *shell)
{
	int		pipe_fd[2];
	int		stdin_save;
	char	*delimiter;

	delimiter = cmd->redirs[i].file;
	if (!setup_heredoc_pipe(pipe_fd))
		return ;
	stdin_save = dup(STDIN_FILENO);
	if (stdin_save == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return ;
	}
	setup_heredoc_signals();
	while (read_heredoc_line(delimiter, pipe_fd[1], shell))
		continue ;
	if (shell->state == 1)
		return ;
	close(pipe_fd[1]);
	if (shell->state == 1)
	{
		dup2(stdin_save, STDIN_FILENO);
		close(stdin_save);
		handle_heredoc_error(pipe_fd, cmd, i, shell);
	}
	else
	{
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		close(stdin_save);
		handle_heredoc_success(pipe_fd, cmd, i);
	}
}
