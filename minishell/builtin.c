#include "minishell.h"

void builtin_cd(char **args) {
	char *path;
	if (args[1] == NULL) {
		path = getenv("HOME");
	} else if (strcmp(args[1], "-") == 0) {
		path = getenv("OLDPWD");
	} else {
		path = args[1];
	}

	if (path && chdir(path) != 0) {
		perror("cd");
	}
}

void builtin_pwd(char **args) {
	(void)args;
	char *cwd = getcwd(NULL, 0);
	if (cwd) {
		printf("%s\n", cwd);
		free(cwd);
	} else {
		perror("pwd");
	}
}

void builtin_exit(char **args) {
	int exit_code = 0;
	if (args[1] != NULL) {
		exit_code = atoi(args[1]);
	}
	printf("exit\n");
	exit(exit_code);
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

static void unset_single_var(char *var_name, t_shell *shell)
{
	int j = 0;
	int len = strlen(var_name);

	while (shell->envp[j])
	{
		if (strncmp(shell->envp[j], var_name, len) == 0
			&& (shell->envp[j][len] == '=' || shell->envp[j][len] == '\0'))
		{
			remove_env_var(shell, j);
			break;
		}
		j++;
	}
}

void builtin_unset(char **args, t_shell *shell)
{
	if (args[1] == NULL)
	{
		printf("unset: not enough arguments\n");
		return;
	}

	int i = 1;
	while (args[i])
	{
		unset_single_var(args[i], shell);
		i++;
	}
}

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void builtin_echo(char **args) {
	int i = 1;
	int nline = 1;

	if (args[1] && strcmp(args[1], "-n") == 0) {
		nline = 0;
		i = 2;
	}

	while (args[i]) {
		write(STDOUT_FILENO, args[i], strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (nline)
		write(STDOUT_FILENO, "\n", 1);
}

void builtin_env(char **args, t_shell *shell) {
	(void)args;
	for (int i = 0; shell->envp[i]; i++) {
		printf("%s\n", shell->envp[i]);
	}
}

static void handle_export_var(char *arg, t_shell *shell)
{
	if (!is_valid_identifier(arg))
	{
		printf("export: '%s': not a valid identifier\n", arg);
		return;
	}

	char *equals = strchr(arg, '=');
	if (equals)
	{
		char *new_var = ft_strdup(arg);
		if (!new_var)
		{
			perror("export: malloc error");
			return;
		}
		add_or_update_env(new_var, shell);
	}
	else
	{
		export_existing_var(arg, shell);
	}
}

void builtin_export(char **args, t_shell *shell)
{
	if (!args[1])
	{
		print_export_format(shell->envp);
		return;
	}

	int i = 1;
	while (args[i])
	{
		handle_export_var(args[i], shell);
		i++;
	}
}
