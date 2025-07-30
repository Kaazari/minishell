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

static char	*read_line_from_stdin(void)
{
	char	buffer[1000];
	int		i;
	int		bytes_read;
	char	c;

	i = 0;
	while (i < 999)
	{
		if (g_signal == SIGINT)
			return (NULL);
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0)
		{
			if (g_signal == SIGINT)
				return (NULL);
			if (bytes_read == 0)
				return (NULL);
			return (NULL);
		}
		if (c == '\n')
		{
			buffer[i] = c;
			buffer[i + 1] = '\0';
			return (ft_strdup(buffer));
		}
		buffer[i] = c;
		i++;
	}
	buffer[i] = '\0';
	return (ft_strdup(buffer));
}

static int	read_heredoc_line(char *delimiter, int pipe_fd, t_shell *shell)
{
	char	*line;

	/* Check if stdin is interactive */
	if (isatty(STDIN_FILENO))
	{
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
	}
	else
	{
		/* Use character-by-character reading for non-interactive input */
		line = read_line_from_stdin();
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
		/* Remove trailing newline if present */
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
	}

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
	{
		close(pipe_fd[0]);
		cmd->redirs[i].is_heredoc_fd = -1;
		restore_main_signals();
		shell->state = 0;
		return ;
	}
	cmd->redirs[i].is_heredoc_fd = pipe_fd[0];
	restore_main_signals();
}
