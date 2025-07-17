/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:31:03 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_command_name(char *cmd)
{
	int	i;

	if (!cmd || !cmd[0])
		return (0);
	if (strcmp(cmd, "!") == 0)
		return (0);
	if (strcmp(cmd, "\\n") == 0 || strcmp(cmd, "\\t") == 0 || strcmp(cmd,
			"\\r") == 0)
		return (0);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\n' || cmd[i] == '\r' || cmd[i] == '\t')
			return (0);
		i++;
	}
	return (1);
}

static void	child_exit(char *path, int status)
{
	if (path)
		free(path);
	exit(status);
}

void	handle_path_not_found(char *command)
{
	if (command[0] == '/' || command[0] == '.')
		printf("minishell: %s: No such file or directory\n", command);
	else
		printf("minishell: %s: command not found\n", command);
	g_shell->exit_status = 127;
}

char	*validate_and_get_path(char **args)
{
	char	*path;

	if (!args || !args[0])
		return (NULL);
	if (strcmp(args[0], "!") == 0 || strcmp(args[0], "\\n") == 0
		|| strcmp(args[0], "\\t") == 0 || strcmp(args[0], "\\r") == 0)
	{
		g_shell->exit_status = 0;
		return (NULL);
	}
	if (!is_valid_command_name(args[0]))
	{
		printf("minishell: %s: command not found\n", args[0]);
		g_shell->exit_status = 127;
		return (NULL);
	}
	path = find_command_in_path(args[0]);
	if (!path)
	{
		handle_path_not_found(args[0]);
		return (NULL);
	}
	return (path);
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	execute_external_commands(char **args, char **envp)
{
	pid_t	pid;
	char	*path;
	int		status;

	path = validate_and_get_path(args);
	if (!path)
		return ;
	g_shell->state = 3;
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execve(path, args, envp);
		perror("execve");
		child_exit(path, 1);
	}
	else if (pid > 0)
		handle_parent_process(pid, &status, path);
	else
	{
		perror("fork");
		free(path);
		g_shell->state = 0;
	}
}

static void	handle_signal_termination(int status)
{
	if (WTERMSIG(status) == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		g_shell->state = 2;
		g_shell->exit_status = 128 + WTERMSIG(status);
	}
	else if (WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_shell->exit_status = 128 + WTERMSIG(status);
	}
}

void	handle_parent_process(pid_t pid, int *status, char *path)
{
	waitpid(pid, status, 0);
	if (WIFSIGNALED(*status))
		handle_signal_termination(*status);
	else if (WIFEXITED(*status))
		g_shell->exit_status = WEXITSTATUS(*status);
	free(path);
	g_shell->state = 0;
}
