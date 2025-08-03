/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Vérifie si une commande contient des heredocs
 *
 * Cette fonction :
 * - Parcourt toutes les redirections de la commande
 * - Vérifie si une redirection de type REDIR_HEREDOC existe
 * - Retourne 1 si un heredoc est trouvé, 0 sinon
 *
 * @param cmd: Commande à vérifier
 * @return 1 si heredoc trouvé, 0 sinon
 */
int	has_heredoc(t_cmd *cmd)
{
	int	i;

	if (!cmd || !cmd->redirs)
		return (0);
	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_HEREDOC)
			return (1);
		i++;
	}
	return (0);
}

/**
 * Ferme tous les descripteurs de fichiers heredoc dans le processus parent
 *
 * Cette fonction :
 * - Parcourt toutes les commandes et leurs redirections
 * - Ferme les descripteurs de fichiers heredoc ouverts
 * - Remet les descripteurs à -1 après fermeture
 *
 * @param commands: Tableau de commandes
 * @param cmd_count: Nombre de commandes
 */
void	close_parent_heredoc_fds(t_cmd **commands, int cmd_count)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmd_count && commands[i])
	{
		j = 0;
		while (j < commands[i]->redir_count)
		{
			if (commands[i]->redirs[j].type == REDIR_HEREDOC
				&& commands[i]->redirs[j].is_heredoc_fd > 0)
			{
				close(commands[i]->redirs[j].is_heredoc_fd);
				commands[i]->redirs[j].is_heredoc_fd = -1;
			}
			j++;
		}
		i++;
	}
}
