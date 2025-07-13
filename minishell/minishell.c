#include "minishell.h"

void	handle_redirection(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (i < cmd->redir_count)
	{
		printf("DEBUG: Processing redirection %d of type %d to file '%s'\n",
			i, cmd->redirs[i].type, cmd->redirs[i].file);

		if (cmd->redirs[i].type == REDIR_OUT || cmd->redirs[i].type == REDIR_APPEND)
		{
			int flags = O_WRONLY | O_CREAT;
			if (cmd->redirs[i].type == REDIR_APPEND)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(cmd->redirs[i].file, flags, 0644);
			printf("DEBUG: Opened output file with fd %d\n", fd);
			if (fd != -1)
			{
				cmd->redirs[i].fd = dup(STDOUT_FILENO);
				printf("DEBUG: Saved stdout (fd %d) and redirecting to fd %d\n",
					cmd->redirs[i].fd, fd);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			else
				perror("open");
		}
		else if (cmd->redirs[i].type == REDIR_IN)
		{
			fd = open(cmd->redirs[i].file, O_RDONLY);
			if (fd != -1)
			{
				cmd->redirs[i].fd = dup(STDIN_FILENO);
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		i++;
	}
}

void	restore_redirections(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_OUT || cmd->redirs[i].type == REDIR_APPEND)
			dup2(cmd->redirs[i].fd, STDOUT_FILENO);
		else if (cmd->redirs[i].type == REDIR_IN)
			dup2(cmd->redirs[i].fd, STDIN_FILENO);
		close(cmd->redirs[i].fd);
		free(cmd->redirs[i].file);
		i++;
	}
	cmd->redir_count = 0;
}

static void execute_builtin_command(char **args, t_shell *shell)
{
	if (strcmp(args[0], "cd") == 0)
		builtin_cd(args);
	else if (strcmp(args[0], "pwd") == 0)
		builtin_pwd(args);
	else if (strcmp(args[0], "export") == 0)
		builtin_export(args, shell);
	else if (strcmp(args[0], "unset") == 0)
		builtin_unset(args, shell);
	else if (strcmp(args[0], "env") == 0)
		builtin_env(args, shell);
	else if (strcmp(args[0], "exit") == 0)
		builtin_exit(args);
	else if (strcmp(args[0], "echo") == 0)
		builtin_echo(args);
	else
		execute_external_commands(args, shell->envp);
}

void execute_command(char **args, t_shell *shell)
{
	t_cmd *cmd;

	if (!args || !args[0])
		return;

	printf("DEBUG: Starting command execution\n");
	cmd = shell->cmd;  // Utiliser la structure cmd existante

	handle_redirection(cmd);
	execute_builtin_command(args, shell);
	restore_redirections(cmd);
}

static void init_shell(t_shell *shell, char **envp)
{
	shell->envp = envp;
}

static int process_empty_input(char *input)
{
	if (strlen(input) == 0)
	{
		free(input);
		return (1);
	}
	return (0);
}

static void process_command(t_shell *shell, char *input)
{
	shell->cmd = tokenize_input(input);
	if (shell->cmd)
	{
		execute_command(shell->cmd->args, shell);
		free_cmd(shell->cmd);
	}
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_shell shell;
	char *input;

	init_shell(&shell, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		add_history(input);
		if (process_empty_input(input))
			continue;
		process_command(&shell, input);
		free(input);
	}
	return (0);
}
