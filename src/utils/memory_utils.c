/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Libère un tableau de chaînes de caractères
 *
 * Cette fonction :
 * - Parcourt le tableau de chaînes
 * - Libère chaque chaîne individuellement
 * - Libère le tableau lui-même
 * - Gère le cas où le tableau est NULL
 *
 * @param args: Tableau de chaînes à libérer
 */
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

/**
 * Libère un environnement (tableau de variables d'environnement)
 *
 * Cette fonction :
 * - Utilise free_args pour libérer l'environnement
 * - Spécialisée pour les environnements dupliqués
 * - Gère le cas où l'environnement est NULL
 *
 * @param envp: Environnement à libérer
 */
void	free_envp(char **envp)
{
	free_args(envp);
}

/**
 * Recherche une commande dans les chemins du PATH
 *
 * Cette fonction :
 * - Parcourt chaque chemin du tableau de chemins
 * - Construit le chemin complet en joignant le chemin et la commande
 * - Vérifie si le fichier existe et est exécutable
 * - Retourne le premier chemin valide trouvé
 * - Libère le tableau de chemins en cas de succès
 *
 * @param command: Nom de la commande à rechercher
 * @param paths: Tableau de chemins à parcourir
 * @return Chemin complet de la commande ou NULL si non trouvée
 */
char	*search_in_path(char *command, char **paths)
{
	char	*dir;
	char	*full_path;
	int		i;

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
