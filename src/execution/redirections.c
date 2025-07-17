/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:31:08 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_shell->state = 1;
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

static void	restore_main_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

static int	setup_heredoc_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

static int	read_heredoc_line(char *delimiter, int pipe_fd)
{
	char	*line;

	line = readline("> ");
	if (!line || g_shell->state == 1)
	{
		if (line)
			free(line);
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

static void	handle_heredoc(t_cmd *cmd, int i)
{
	int		pipe_fd[2];
	char	*delimiter;
	int		stdin_backup;

	delimiter = cmd->redirs[i].file;
	if (!setup_heredoc_pipe(pipe_fd))
		return ;
	stdin_backup = dup(STDIN_FILENO);
	setup_heredoc_signals();
	while (read_heredoc_line(delimiter, pipe_fd[1]))
		continue ;
	close(pipe_fd[1]);
	if (g_shell->state == 1)
	{
		close(pipe_fd[0]);
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		restore_main_signals();
		g_shell->state = 0;
		return ;
	}
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	close(stdin_backup);
	restore_main_signals();
}

void	handle_redirections(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_OUT
			|| cmd->redirs[i].type == REDIR_APPEND)
		{
			handle_output_redirection(cmd, i);
		}
		else if (cmd->redirs[i].type == REDIR_IN)
		{
			handle_input_redirection(cmd, i);
		}
		else if (cmd->redirs[i].type == REDIR_HEREDOC)
		{
			handle_heredoc(cmd, i);
		}
		i++;
	}
}
