#include "minishell.h"

void execute_external_commands(char **args, char **envp)
{
	pid_t pid;
	char *path;

	if (!args || !args[0])
		return;

	path = find_command_in_path(args[0]);
	if (!path)
	{
		printf("minishell: %s: command not found\n", args[0]);
		return;
	}

	pid = fork();
	if (pid == 0)
	{
		execve(path, args, envp);
		perror("execve");
		free(path);
		exit(1);
	}
	else if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
		free(path);
	}
	else
	{
		perror("fork");
		free(path);
	}
}


