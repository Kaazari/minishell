/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:31:20 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*check_absolute_path(char *command)
{
	if (command[0] == '/' || command[0] == '.')
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	return (NULL);
}

char	*find_command_in_path(char *command)
{
	char	*path;
	char	*result;
	char	**paths;

	result = check_absolute_path(command);
	if (result)
		return (result);
	path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	result = search_in_path(command, paths);
	if (!result)
		free_args(paths);
	return (result);
}

static void	print_export_line(char *env_var)
{
	char	*equal_sign;
	char	*name;
	char	*value;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		name = ft_substr(env_var, 0, equal_sign - env_var);
		value = ft_substr(env_var, equal_sign - env_var + 1, ft_strlen(env_var)
				- (equal_sign - env_var + 1));
		printf("declare -x %s=\"%s\"\n", name, value);
		free(name);
		free(value);
	}
	else
		printf("declare -x %s\n", env_var);
}

void	print_export_format(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		print_export_line(envp[i]);
		i++;
	}
}

int	update_existing_env_var(char *var, t_shell *shell)
{
	int		i;
	char	*equal_pos;
	int		var_len;

	i = 0;
	equal_pos = ft_strchr(var, '=');
	if (!equal_pos)
		return (0);
	var_len = equal_pos - var;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var, var_len) == 0
			&& (shell->envp[i][var_len] == '='
				|| shell->envp[i][var_len] == '\0'))
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strdup(var);
			return (1);
		}
		i++;
	}
	return (0);
}
