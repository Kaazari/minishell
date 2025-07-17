/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:31:23 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_new_env_var(char *var, t_shell *shell)
{
	int		i;
	char	**new_envp;
	int		len;

	len = 0;
	while (shell->envp[len])
		len++;
	new_envp = malloc(sizeof(char *) * (len + 2));
	i = 0;
	while (shell->envp[i])
	{
		new_envp[i] = ft_strdup(shell->envp[i]);
		i++;
	}
	new_envp[i] = ft_strdup(var);
	new_envp[i + 1] = NULL;
	free_args(shell->envp);
	shell->envp = new_envp;
}

void	add_or_update_env(char *var, t_shell *shell)
{
	if (update_existing_env_var(var, shell))
		return ;
	add_new_env_var(var, shell);
}

void	export_existing_var(char *var, t_shell *shell)
{
	int		i;
	char	**new_envp;
	int		len;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var, ft_strlen(var)) == 0)
			return ;
		i++;
	}
	len = 0;
	while (shell->envp[len])
		len++;
	new_envp = malloc(sizeof(char *) * (len + 2));
	i = 0;
	while (shell->envp[i])
	{
		new_envp[i] = ft_strdup(shell->envp[i]);
		i++;
	}
	new_envp[i] = ft_strdup(var);
	new_envp[i + 1] = NULL;
	free_args(shell->envp);
	shell->envp = new_envp;
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	*search_in_path(char *command, char **paths)
{
	char *dir;
	char *full_path;
	int i;

	i = 0;
	while (paths[i])
	{
		dir = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(dir, command);
		free(dir);
		if (access(full_path, X_OK) == 0)
		{
			free_args(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}