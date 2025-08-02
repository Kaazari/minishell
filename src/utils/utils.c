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

/**
 * Vérifie si une commande est un chemin absolu et exécutable
 *
 * Cette fonction :
 * - Vérifie si la commande commence par '/' ou '.'
 * - Teste si le fichier existe et est exécutable avec access()
 * - Retourne une copie de la commande si elle est valide
 *
 * @param command: Nom de la commande à vérifier
 * @return Copie de la commande si elle est un chemin absolu valide, NULL sinon
 *
 * Note: Cette fonction utilise ft_strdup pour retourner une copie de la commande
 */
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

/**
 * Trouve le chemin complet d'une commande dans le PATH
 *
 * Cette fonction :
 * - Vérifie d'abord si la commande est un chemin absolu
 * - Récupère la variable d'environnement PATH
 * - Divise le PATH en chemins individuels
 * - Recherche la commande dans chaque chemin
 * - Retourne le premier chemin valide trouvé
 *
 * @param command: Nom de la commande à rechercher
 * @return Chemin complet de la commande ou NULL si non trouvée
 *
 * Note: Cette fonction gère la libération mémoire du tableau de chemins
 * en cas d'échec de recherche
 */
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

/**
 * Affiche une ligne de variable d'environnement au format export
 *
 * Cette fonction :
 * - Trouve le signe égal dans la variable
 * - Extrait le nom et la valeur de la variable
 * - Affiche au format "declare -x NOM=\"VALEUR\""
 * - Gère les variables sans valeur (sans signe égal)
 *
 * @param env_var: Variable d'environnement à afficher
 *
 * Note: Cette fonction alloue et libère la mémoire pour les sous-chaînes
 */
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

/**
 * Affiche toutes les variables d'environnement au format export
 *
 * Cette fonction :
 * - Parcourt toutes les variables d'environnement
 * - Affiche chaque variable au format export
 * - Trie les variables par ordre alphabétique (géré par l'appelant)
 *
 * @param envp: Tableau de variables d'environnement
 *
 * Note: Cette fonction utilise print_export_line pour chaque variable
 */
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

/**
 * Met à jour une variable d'environnement existante
 *
 * Cette fonction :
 * - Trouve le signe égal dans la nouvelle variable
 * - Recherche la variable existante par son nom
 * - Remplace la valeur de la variable existante
 * - Libère l'ancienne valeur et alloue la nouvelle
 *
 * @param var: Nouvelle variable au format "NOM=VALEUR"
 * @param shell: Structure shell contenant l'environnement
 * @return 1 si la variable a été mise à jour, 0 si elle n'existe pas
 *
 * Note: Cette fonction ne gère que les variables avec un signe égal.
 * Les variables sans valeur ne sont pas mises à jour.
 */
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
