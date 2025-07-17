/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:27:04 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_shell		*g_shell = NULL;

static void	cleanup_pipex(t_shell *shell)
{
	if (shell->pipex)
	{
		if (shell->pipex->pipe_fds)
			free(shell->pipex->pipe_fds);
		free(shell->pipex);
	}
}

static void	cleanup_cmd(t_shell *shell)
{
	if (shell->cmd)
	{
		free_cmd(shell->cmd);
		shell->cmd = NULL;
	}
}

static void	cleanup_envp(t_shell *shell)
{
	char	**tmp;

	if (shell->local_envp)
	{
		tmp = shell->local_envp;
		while (*tmp)
			free(*tmp++);
		free(shell->local_envp);
	}
}

void	clean_exit(int status)
{
	if (g_shell)
	{
		cleanup_pipex(g_shell);
		cleanup_cmd(g_shell);
		cleanup_envp(g_shell);
		g_shell = NULL;
	}
	clear_history();
	exit(status);
}

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		if (g_shell->state == 3)
		{
			write(STDOUT_FILENO, "\n", 1);
			g_shell->state = 1;
		}
		else
		{
			write(STDOUT_FILENO, "\n", 1);
			g_shell->state = 1;
		}
	}
}

static void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

static void	execute_builtin_command(char **args, t_shell *shell)
{
	if (strcmp(args[0], "cd") == 0)
		shell->exit_status = builtin_cd(args);
	else if (strcmp(args[0], "pwd") == 0)
		shell->exit_status = builtin_pwd(args);
	else if (strcmp(args[0], "echo") == 0)
		shell->exit_status = builtin_echo(args);
	else if (strcmp(args[0], "env") == 0)
		shell->exit_status = builtin_env(args, shell);
	else if (strcmp(args[0], "export") == 0)
		shell->exit_status = builtin_export(args, shell);
	else if (strcmp(args[0], "unset") == 0)
		shell->exit_status = builtin_unset(args, shell);
	else if (strcmp(args[0], "exit") == 0)
		builtin_exit(args);
	else if (strcmp(args[0], ":") == 0)
		shell->exit_status = builtin_colon(args);
	else
		execute_external_commands(args, shell->envp);
}

void	execute_shell_command(char **args, t_shell *shell)
{
	int	old_state;

	old_state = shell->state;
	shell->state = 3;
	handle_redirections(shell->cmd);
	if (args && args[0])
		execute_builtin_command(args, shell);
	restore_redirections(shell->cmd);
	shell->state = old_state;
}

static void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = envp;
	shell->local_envp = NULL;
	shell->state = 0;
	shell->exit_status = 0;
	shell->pipex = malloc(sizeof(t_pipex));
	if (!shell->pipex)
	{
		printf("Error: Failed to allocate memory for pipex\n");
		exit(1);
	}
	shell->pipex->pipe_fds = NULL;
	shell->cmd = create_cmd();
	if (!shell->cmd)
	{
		free(shell->pipex);
		printf("Error: Failed to allocate memory for cmd\n");
		exit(1);
	}
	g_shell = shell;
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	setup_signals();
	init_shell(&shell, envp);
	main_shell_loop(&shell);
	return (0);
}
