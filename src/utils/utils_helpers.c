/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Vérifie si une chaîne d'entrée est vide ou ne contient que des espaces
 *
 * Cette fonction :
 * - Vérifie si la chaîne est vide (longueur 0)
 * - Parcourt la chaîne pour vérifier qu'elle ne contient que des espaces,
 *   tabulations, retours à la ligne ou retours chariot
 * - Retourne 1 si la chaîne est vide, 0 sinon
 *
 * @param input: Chaîne d'entrée à vérifier
 * @return 1 si la chaîne est vide, 0 sinon
 */
int	process_empty_input(char *input)
{
	int	i;

	i = 0;
	if (ft_strlen(input) == 0)
		return (1);
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n'
			&& input[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

/**
 * Vérifie si une variable d'environnement existe déjà
 *
 * Cette fonction :
 * - Parcourt toutes les variables d'environnement
 * - Compare le nom de la variable avec les variables existantes
 * - Retourne 1 si la variable existe, 0 sinon
 *
 * @param var: Nom de la variable à vérifier
 * @param shell: Structure shell contenant l'environnement
 * @return 1 si la variable existe, 0 sinon
 */
static int	var_already_exists(char *var, t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var, ft_strlen(var)) == 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * Exporte une variable d'environnement existante
 *
 * Cette fonction :
 * - Vérifie si la variable existe déjà dans l'environnement
 * - Ajoute la variable si elle n'existe pas
 * - Ne fait rien si la variable existe déjà
 *
 * @param var: Variable à exporter
 * @param shell: Structure shell contenant l'environnement
 */
void	export_existing_var(char *var, t_shell *shell)
{
	if (var_already_exists(var, shell))
		return ;
	add_new_env_var(var, shell);
}
