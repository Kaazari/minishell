/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**allocate_new_envp(int len)
{
	char	**new_envp;

	new_envp = malloc(sizeof(char *) * (len + 2));
	if (!new_envp)
	{
		perror("malloc");
		return (NULL);
	}
	return (new_envp);
}

static int	copy_existing_env(char **new_envp, t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		new_envp[i] = ft_strdup(shell->envp[i]);
		if (!new_envp[i])
		{
			while (i > 0)
				free(new_envp[--i]);
			free(new_envp);
			perror("ft_strdup");
			return (0);
		}
		i++;
	}
	return (i);
}

void	add_new_env_var(char *var, t_shell *shell)
{
	int		i;
	char	**new_envp;
	int		len;

	len = 0;
	while (shell->envp[len])
		len++;
	new_envp = allocate_new_envp(len);
	if (!new_envp)
		return ;
	i = copy_existing_env(new_envp, shell);
	if (!i && shell->envp[0])
		return ;
	new_envp[i] = ft_strdup(var);
	if (!new_envp[i])
	{
		while (i > 0)
			free(new_envp[--i]);
		free(new_envp);
		perror("ft_strdup");
		return ;
	}
	new_envp[i + 1] = NULL;
	free_envp(shell->envp);
	shell->envp = new_envp;
}

void	add_or_update_env(char *var, t_shell *shell)
{
	if (update_existing_env_var(var, shell))
		return ;
	add_new_env_var(var, shell);
}

char	*get_env_value(char *var, char **envp)
{
	int		i;
	int		len;
	char	*value;

	i = 0;
	len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
		{
			value = envp[i] + len + 1;
			return (value);
		}
		i++;
	}
	return (NULL);
}
