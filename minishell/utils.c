#include "minishell.h"

static char *try_path(char *dir, char *command)
{
	char *full_path;

	full_path = malloc(strlen(dir) + strlen(command) + 2);
	if (!full_path)
		return NULL;

	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, command);

	if (access(full_path, X_OK) == 0)
		return full_path;

	free(full_path);
	return NULL;
}

char *find_command_in_path(char *command)
{
	char *path_env = getenv("PATH");
	char *path_copy;
	char *dir;
	char *full_path;

	if (!path_env)
		return NULL;

	path_copy = malloc(strlen(path_env) + 1);
	if (!path_copy)
		return NULL;
	strcpy(path_copy, path_env);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		full_path = try_path(dir, command);
		if (full_path)
		{
			free(path_copy);
			return full_path;
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return NULL;
}

void print_export_format(char **envp)
{
	for (int i = 0; envp[i]; i++)
	{
		printf("declare -x %s\n", envp[i]);
	}
}

void add_or_update_env(char *var, t_shell *shell)
{
	(void)shell;
	char *equal_pos = strchr(var, '=');
	if (equal_pos)
	{
		*equal_pos = '\0';
		setenv(var, equal_pos + 1, 1);
		*equal_pos = '=';
	}
}

void export_existing_var(char *var, t_shell *shell)
{
	(void)shell;
	char *value = getenv(var);
	if (value)
	{
		setenv(var, value, 1);
	}
}
