#include "minishell.h"

int	builtin_cd(char **args)
{
	char	*path;

	if (args[1] == NULL)
		path = getenv("HOME");
	else if (strcmp(args[1], "-") == 0)
		path = getenv("OLDPWD");
	else
		path = args[1];
	if (path && chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_pwd(char **args)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

static void	remove_env_var(t_shell *shell, int index)
{
	free(shell->envp[index]);
	while (shell->envp[index + 1])
	{
		shell->envp[index] = shell->envp[index + 1];
		index++;
	}
	shell->envp[index] = NULL;
}

static void	unset_single_var(char *var_name, t_shell *shell)
{
	int	j;
	int	len;

	j = 0;
	len = strlen(var_name);
	while (shell->envp[j])
	{
		if (strncmp(shell->envp[j], var_name, len) == 0
			&& (shell->envp[j][len] == '=' || shell->envp[j][len] == '\0'))
		{
			remove_env_var(shell, j);
			break ;
		}
		j++;
	}
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	if (args[1] == NULL)
	{
		printf("unset: not enough arguments\n");
		return (1);
	}
	i = 1;
	while (args[i])
	{
		unset_single_var(args[i], shell);
		i++;
	}
	return (0);
}

int	builtin_echo(char **args)
{
	int	i;
	int	nline;

	i = 1;
	nline = 1;
	if (args[1] && strcmp(args[1], "-n") == 0)
	{
		nline = 0;
		i = 2;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (nline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	builtin_env(char **args, t_shell *shell)
{
	int	i;

	(void)args;
	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	return (0);
}

static void	handle_export_var(char *arg, t_shell *shell)
{
	if (strchr(arg, '='))
		add_or_update_env(arg, shell);
	else
		export_existing_var(arg, shell);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
	{
		print_export_format(shell->envp);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		handle_export_var(args[i], shell);
		i++;
	}
	return (0);
}

int	builtin_colon(char **args)
{
	(void)args;
	return (0);
}

void	builtin_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	if (args[1] != NULL)
		exit_code = atoi(args[1]);
	printf("exit\n");
	exit(exit_code);
}
