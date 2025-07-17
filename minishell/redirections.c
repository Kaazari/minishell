#include "minishell.h"

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

static void	handle_heredoc(t_cmd *cmd, int i)
{
	int		pipe_fd[2];
	char	*line;
	char	*delimiter;
	int		stdin_backup;

	delimiter = cmd->redirs[i].file;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return ;
	}
	stdin_backup = dup(STDIN_FILENO);
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line || g_shell->state == 1)
		{
			if (line)
				free(line);
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			if (g_shell->state == 1)
			{
				dup2(stdin_backup, STDIN_FILENO);
				close(stdin_backup);
				restore_main_signals();
				g_shell->state = 0;
				return ;
			}
			break ;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	close(stdin_backup);
	restore_main_signals();
}

void	handle_output_redirection(t_cmd *cmd, int i)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->redirs[i].type == REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->redirs[i].file, flags, 0644);
	if (fd != -1)
	{
		cmd->redirs[i].fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else
		perror("open");
}

void	handle_input_redirection(t_cmd *cmd, int i)
{
	int	fd;

	fd = open(cmd->redirs[i].file, O_RDONLY);
	if (fd != -1)
	{
		cmd->redirs[i].fd = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
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

void	restore_output_redirection(t_cmd *cmd, int i)
{
	dup2(cmd->redirs[i].fd, STDOUT_FILENO);
	close(cmd->redirs[i].fd);
}

void	restore_input_redirection(t_cmd *cmd, int i)
{
	dup2(cmd->redirs[i].fd, STDIN_FILENO);
	close(cmd->redirs[i].fd);
}

void	restore_redirections(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_OUT
			|| cmd->redirs[i].type == REDIR_APPEND)
		{
			restore_output_redirection(cmd, i);
		}
		else if (cmd->redirs[i].type == REDIR_IN)
		{
			restore_input_redirection(cmd, i);
		}
		free(cmd->redirs[i].file);
		i++;
	}
	cmd->redir_count = 0;
}
