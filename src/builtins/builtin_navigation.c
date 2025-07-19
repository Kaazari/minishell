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

static void	update_pwd_vars(t_shell *shell, char *old_pwd)
{
	char	*new_pwd;
	char	*pwd_var;
	char	*oldpwd_var;

	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		pwd_var = ft_strjoin("PWD=", new_pwd);
		add_or_update_env(pwd_var, shell);
		free(pwd_var);
		free(new_pwd);
	}
	if (old_pwd)
	{
		oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
		add_or_update_env(oldpwd_var, shell);
		free(oldpwd_var);
		free(old_pwd);
	}
}

static char	*get_cd_path(char **args)
{
	char	*path;

	if (args[1] == NULL)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			printf("cd: OLDPWD not set\n");
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	path = get_cd_path(args);
	if (!path)
	{
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	update_pwd_vars(shell, old_pwd);
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
