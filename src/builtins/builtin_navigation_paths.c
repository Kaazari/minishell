/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_navigation_paths.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_tilde(char *path, t_shell *shell);

char	*get_home_path(void)
{
	char	*path;

	path = getenv("HOME");
	if (!path)
	{
		printf("cd: HOME not set\n");
		return (NULL);
	}
	return (ft_strdup(path));
}

char	*get_oldpwd_path(void)
{
	char	*path;

	path = getenv("OLDPWD");
	if (!path)
	{
		printf("cd: OLDPWD not set\n");
		return (NULL);
	}
	return (ft_strdup(path));
}

/**
 * Gère un chemin personnalisé avec expansion du tilde
 *
 * Cette fonction :
 * - Prend le chemin depuis les arguments
 * - Développe le tilde si présent
 * - Retourne le chemin final
 *
 * @param args: Arguments de la commande
 * @param shell: Structure shell
 * @return Chemin traité ou NULL en cas d'erreur
 */
static char	*handle_custom_path(char **args, t_shell *shell)
{
	char	*path;
	char	*expanded_path;

	path = args[1];
	expanded_path = expand_tilde(path, shell);
	if (!expanded_path)
		return (NULL);
	if (expanded_path != path)
		return (expanded_path);
	else
	{
		free(expanded_path);
		return (ft_strdup(args[1]));
	}
}

/**
 * Détermine le chemin à utiliser pour la commande cd
 *
 * Cette fonction :
 * - Gère le cas sans argument (utilise HOME)
 * - Gère le cas "-" (utilise OLDPWD)
 * - Gère les chemins personnalisés avec expansion du tilde
 *
 * @param args: Arguments de la commande
 * @param shell: Structure shell
 * @return Chemin à utiliser ou NULL en cas d'erreur
 */
char	*get_cd_path(char **args, t_shell *shell)
{
	if (args[1] == NULL)
		return (get_home_path());
	else if (ft_strncmp(args[1], "-", 2) == 0)
		return (get_oldpwd_path());
	else
		return (handle_custom_path(args, shell));
}
