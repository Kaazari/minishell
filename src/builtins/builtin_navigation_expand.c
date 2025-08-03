/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_navigation_expand.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Récupère le répertoire de base de l'utilisateur
 *
 * Cette fonction :
 * - Récupère la variable d'environnement HOME
 * - Affiche un message d'erreur si HOME n'est pas définie
 * - Retourne le chemin du répertoire de base
 *
 * @return Chemin du répertoire de base ou NULL si non défini
 */
static char	*get_home_directory(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		printf("cd: HOME not set\n");
		return (NULL);
	}
	return (home);
}

/**
 * Développe le chemin avec le répertoire de base
 *
 * Cette fonction :
 * - Gère le cas où le chemin est juste "~"
 * - Joint le répertoire de base avec le reste du chemin
 * - Retourne le chemin développé
 *
 * @param path: Chemin à développer
 * @param home: Répertoire de base
 * @return Chemin développé
 */
static char	*expand_home_path(char *path, char *home)
{
	char	*expanded_path;

	if (path[1] == '\0')
		return (ft_strdup(home));
	else
	{
		expanded_path = ft_strjoin(home, path + 1);
		return (expanded_path);
	}
}

/**
 * Développe le tilde (~) dans un chemin
 *
 * Cette fonction :
 * - Vérifie si le chemin commence par "~"
 * - Récupère le répertoire de base de l'utilisateur
 * - Développe le tilde en remplaçant par le chemin complet
 * - Retourne le chemin développé ou le chemin original
 *
 * @param path: Chemin à traiter
 * @param shell: Structure shell (non utilisée)
 * @return Chemin développé ou NULL en cas d'erreur
 */
char	*expand_tilde(char *path, t_shell *shell)
{
	char	*home;

	(void)shell;
	if (!path || path[0] != '~')
		return (ft_strdup(path));
	home = get_home_directory();
	if (!home)
		return (NULL);
	if (path[1] == '\0' || path[1] == '/')
		return (expand_home_path(path, home));
	return (ft_strdup(path));
}
