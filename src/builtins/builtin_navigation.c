/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_navigation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	update_pwd_vars(t_shell *shell)
{
	char	*pwd_var;
	char	*oldpwd_var;

	if (shell->pwd)
	{
		pwd_var = ft_strjoin("PWD=", shell->pwd);
		add_or_update_env(pwd_var, shell);
		free(pwd_var);
	}
	if (shell->oldpwd)
	{
		oldpwd_var = ft_strjoin("OLDPWD=", shell->oldpwd);
		add_or_update_env(oldpwd_var, shell);
		free(oldpwd_var);
	}
}

static char	*expand_tilde(char *path, t_shell *shell)
{
	char	*home;
	char	*expanded_path;

	(void)shell; /* Unused parameter for now */
	if (!path || path[0] != '~')
		return (ft_strdup(path));

	home = getenv("HOME");
	if (!home)
	{
		printf("cd: HOME not set\n");
		return (NULL);
	}

	if (path[1] == '\0' || path[1] == '/')
	{
		if (path[1] == '\0')
			return (ft_strdup(home));
		else
		{
			expanded_path = ft_strjoin(home, path + 1);
			return (expanded_path);
		}
	}

	/* For now, only handle ~ and ~/path */
	/* TODO: Handle ~username expansion if needed */
	return (ft_strdup(path));
}

static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*path;
	char	*expanded_path;

	if (args[1] == NULL)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
		return (ft_strdup(path));
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			printf("cd: OLDPWD not set\n");
			return (NULL);
		}
		return (ft_strdup(path));
	}
	else
	{
		path = args[1];
		expanded_path = expand_tilde(path, shell);
		if (!expanded_path)
			return (NULL);
		if (expanded_path != path) /* If expansion occurred */
		{
			return (expanded_path);
		}
		else
		{
			free(expanded_path);
			return (ft_strdup(args[1]));
		}
	}
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	path = get_cd_path(args, shell);
	if (!path)
	{
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		free(path);
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	free(shell->oldpwd);
	shell->oldpwd = old_pwd;
	free(shell->pwd);
	shell->pwd = getcwd(NULL, 0);
	if (!shell->pwd)
	{
		perror("cd: getcwd failed");
		free(path);
		return (1);
	}
	update_pwd_vars(shell);
	free(path);
	return (0);
}

int	builtin_pwd(t_shell *shell)
{
	char	*cwd;

	(void)shell;
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
