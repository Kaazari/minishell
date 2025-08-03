/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_navigation_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Met à jour les variables d'environnement PWD et OLDPWD
 *
 * Cette fonction :
 * - Crée les variables d'environnement PWD et OLDPWD
 * - Les ajoute ou les met à jour dans l'environnement du shell
 * - Libère la mémoire des variables temporaires
 *
 * @param shell: Structure shell contenant l'état
 */
void	update_pwd_vars(t_shell *shell)
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

/**
 * Gère les erreurs de la commande cd
 *
 * Cette fonction :
 * - Libère la mémoire de l'ancien répertoire
 * - Retourne le code d'erreur approprié
 *
 * @param old_pwd: Ancien répertoire de travail
 * @return Code d'erreur (1)
 */
int	handle_cd_error(char *old_pwd)
{
	if (old_pwd)
		free(old_pwd);
	return (1);
}

/**
 * Gère les erreurs de chdir
 *
 * Cette fonction :
 * - Affiche l'erreur avec perror
 * - Libère la mémoire du chemin et de l'ancien répertoire
 * - Retourne le code d'erreur approprié
 *
 * @param path: Chemin qui a causé l'erreur
 * @param old_pwd: Ancien répertoire de travail
 * @return Code d'erreur (1)
 */
int	handle_chdir_error(char *path, char *old_pwd)
{
	perror("cd");
	free(path);
	if (old_pwd)
		free(old_pwd);
	return (1);
}

/**
 * Met à jour les variables PWD et OLDPWD du shell
 *
 * Cette fonction :
 * - Sauvegarde l'ancien répertoire dans OLDPWD
 * - Récupère le nouveau répertoire de travail
 * - Met à jour la variable PWD du shell
 *
 * @param shell: Structure shell
 * @param old_pwd: Ancien répertoire de travail
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
int	update_shell_pwd(t_shell *shell, char *old_pwd)
{
	free(shell->oldpwd);
	shell->oldpwd = old_pwd;
	free(shell->pwd);
	shell->pwd = getcwd(NULL, 0);
	if (!shell->pwd)
	{
		perror("cd: getcwd failed");
		return (1);
	}
	return (0);
}

/**
 * Change le répertoire de travail
 *
 * Cette fonction :
 * - Récupère l'ancien répertoire de travail
 * - Obtient le nouveau chemin via get_cd_path
 * - Change de répertoire avec chdir
 * - Met à jour les variables PWD et OLDPWD
 *
 * @param args: Arguments de la commande cd
 * @param shell: Structure shell
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	path = get_cd_path(args, shell);
	if (!path)
		return (handle_cd_error(old_pwd));
	if (chdir(path) != 0)
		return (handle_chdir_error(path, old_pwd));
	if (update_shell_pwd(shell, old_pwd) != 0)
	{
		free(path);
		return (1);
	}
	update_pwd_vars(shell);
	free(path);
	return (0);
}
