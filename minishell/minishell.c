#include "minishell.h"

t_shell	*g_shell = NULL;

static void	clean_exit(int status)
{
	char	**tmp;

	if (g_shell)
	{
		if (g_shell->pipex)
		{
			if (g_shell->pipex->pipe_fds)
				free(g_shell->pipex->pipe_fds);
			free(g_shell->pipex);
		}
		if (g_shell->cmd)
		{
			free_cmd(g_shell->cmd);
			g_shell->cmd = NULL;
		}
		if (g_shell->local_envp)
		{
			tmp = g_shell->local_envp;
			while (*tmp)
				free(*tmp++);
			free(g_shell->local_envp);
		}
	}
	rl_clear_history();
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
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
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
	shell->state = 0;
	shell->exit_status = 0;
	shell->pipex = malloc(sizeof(t_pipex));
	if (!shell->pipex)
		return ;
	shell->pipex->pipe_fds = NULL;
	shell->cmd = create_cmd();
	if (!shell->cmd)
	{
		free(shell->pipex);
		return ;
	}
	g_shell = shell;
}

static int	process_empty_input(char *input)
{
	int	i;

	i = 0;
	if (strlen(input) == 0)
		return (1);
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n'
			&& input[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

static void	handle_single_command(t_shell *shell, t_cmd **commands)
{
	shell->cmd = commands[0];
	execute_shell_command(commands[0]->args, shell);
	free(commands);
}

static void	handle_piped_commands(t_shell *shell, t_cmd **commands, int count)
{
	int	i;

	execute_piped_commands(shell, commands, count);
	i = 0;
	while (i < count)
	{
		free_cmd(commands[i]);
		i++;
	}
	free(commands);
}

static void	process_command(t_shell *shell, char *input)
{
	int		cmd_count;
	t_cmd	**commands;

	commands = tokenize_piped_commands(input, &cmd_count);
	if (!commands || !commands[0])
	{
		if (commands)
			free(commands);
		return ;
	}
	if (cmd_count == 1)
		handle_single_command(shell, commands);
	else
		handle_piped_commands(shell, commands, cmd_count);
}

void	main_shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell->state = 0;
		input = readline("minishell> ");
		if (!input)
			clean_exit(0);
		if (shell->state == 1)
		{
			free(input);
			continue ;
		}
		if (strlen(input) > 0)
			add_history(input);
		if (process_empty_input(input))
			continue ;
		process_command(shell, input);
		free(input);
		if (shell->state == 2)
			clean_exit(131);
	}
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
