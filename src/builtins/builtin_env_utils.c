/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Affiche toutes les variables d'environnement
 *
 * Cette fonction :
 * - Parcourt toutes les variables d'environnement
 * - Affiche chaque variable sur une ligne séparée
 * - Retourne 0 en cas de succès
 *
 * @param shell: Structure shell contenant l'environnement
 * @return 0 en cas de succès
 */
int	builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	return (0);
}

/**
 * Gère l'export d'une variable d'environnement
 *
 * Cette fonction :
 * - Vérifie si la variable contient un signe égal
 * - Ajoute ou met à jour la variable selon le cas
 * - Exporte une variable existante si elle n'a pas de signe égal
 *
 * @param arg: Variable à exporter
 * @param shell: Structure shell
 */
static void	handle_export_var(char *arg, t_shell *shell)
{
	if (ft_strchr(arg, '='))
		add_or_update_env(arg, shell);
	else
	{
		if (getenv(arg))
			export_existing_var(arg, shell);
	}
}

/**
 * Exporte des variables d'environnement
 *
 * Cette fonction :
 * - Affiche toutes les variables au format export si aucun argument
 * - Traite chaque argument comme une variable à exporter
 * - Gère les variables avec et sans signe égal
 *
 * @param args: Arguments de la commande export
 * @param shell: Structure shell
 * @return 0 en cas de succès
 */
int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
	{
		print_export_format(shell->envp);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		handle_export_var(args[i], shell);
		i++;
	}
	return (0);
}

/**
 * Affiche le répertoire de travail actuel
 *
 * Cette fonction :
 * - Récupère le répertoire de travail actuel avec getcwd
 * - Affiche le chemin sur la sortie standard
 * - Libère la mémoire allouée
 *
 * @param shell: Structure shell (non utilisée)
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
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

char	**dup_envp(char **envp)
{
	int		i;
	int		count;
	char	**copy;

	count = 0;
	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i > 0)
				free(copy[--i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}
