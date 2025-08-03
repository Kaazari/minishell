/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Supprime une variable d'environnement
 *
 * Cette fonction :
 * - Trouve la variable par son nom
 * - Déplace les variables suivantes pour combler le trou
 * - Libère la mémoire de la variable supprimée
 *
 * @param shell: Structure shell
 * @param index: Index de la variable à supprimer
 */
static void	remove_env_var(t_shell *shell, int index)
{
	free(shell->envp[index]);
	while (shell->envp[index + 1])
	{
		shell->envp[index] = shell->envp[index + 1];
		index++;
	}
	shell->envp[index] = NULL;
}

/**
 * Supprime une variable d'environnement spécifique
 *
 * Cette fonction :
 * - Parcourt toutes les variables d'environnement
 * - Compare le nom de la variable avec la variable à supprimer
 * - Supprime la variable si elle correspond exactement
 *
 * @param var_name: Nom de la variable à supprimer
 * @param shell: Structure shell
 */
static void	unset_single_var(char *var_name, t_shell *shell)
{
	int	j;
	int	len;

	j = 0;
	len = ft_strlen(var_name);
	while (shell->envp[j])
	{
		if (ft_strncmp(shell->envp[j], var_name, len) == 0
			&& (shell->envp[j][len] == '=' || shell->envp[j][len] == '\0'))
		{
			remove_env_var(shell, j);
			break ;
		}
		j++;
	}
}

/**
 * Supprime des variables d'environnement
 *
 * Cette fonction :
 * - Traite chaque argument comme une variable à supprimer
 * - Ignore la commande si aucun argument n'est fourni
 * - Supprime chaque variable spécifiée
 *
 * @param args: Arguments de la commande unset
 * @param shell: Structure shell
 * @return 0 en cas de succès
 */
int	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	if (args[1] == NULL)
		return (0);
	i = 1;
	while (args[i])
	{
		unset_single_var(args[i], shell);
		i++;
	}
	return (0);
}
