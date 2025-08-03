/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_cmd_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:00:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Alloue et initialise le tableau d'arguments
 *
 * @param cmd: Structure de commande
 * @return 0 en cas de succès, -1 en cas d'échec
 */
int	init_cmd_args(t_cmd *cmd)
{
	int	i;

	cmd->args = malloc(sizeof(char *) * 64);
	if (!cmd->args)
	{
		cmd->redirs = NULL;
		return (-1);
	}
	i = 0;
	while (i < 64)
	{
		cmd->args[i] = NULL;
		i++;
	}
	return (0);
}

/**
 * Alloue et initialise le tableau de redirections
 *
 * @param cmd: Structure de commande
 * @return 0 en cas de succès, -1 en cas d'échec
 */
int	init_cmd_redirs(t_cmd *cmd)
{
	cmd->redirs = malloc(sizeof(t_redir) * 10);
	if (!cmd->redirs)
	{
		free(cmd->args);
		cmd->args = NULL;
		return (-1);
	}
	return (0);
}

/**
 * Initialise les valeurs par défaut de la commande
 *
 * @param cmd: Structure de commande
 */
void	init_cmd_defaults(t_cmd *cmd)
{
	cmd->redir_count = 0;
	cmd->stdin_backup = -1;
	cmd->stdout_backup = -1;
	cmd->logical_operator = 0;
}
