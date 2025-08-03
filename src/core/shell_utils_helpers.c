/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Vérifie si des opérateurs logiques sont présents dans les commandes
 *
 * @param commands: Tableau de commandes à vérifier
 * @param cmd_count: Nombre de commandes dans le tableau
 * @return 1 si des opérateurs logiques sont trouvés, 0 sinon
 *
 * Note: logical_operator != 0 indique la présence d'un opérateur || ou &&
 */
int	has_logical_operators(t_cmd **commands, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count && commands[i])
	{
		if (commands[i]->logical_operator != 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * Vérifie si une commande est valide
 *
 * @param commands: Tableau de commandes à vérifier
 * @param cmd_count: Nombre de commandes dans le tableau
 * @return 1 si la commande est valide, 0 sinon
 */
int	is_valid_command(t_cmd **commands, int cmd_count)
{
	(void)cmd_count;
	if (!commands[0] || !commands[0]->args || !commands[0]->args[0]
		|| ft_strlen(commands[0]->args[0]) == 0)
		return (0);
	return (1);
}

/**
 * Gère l'erreur de commande non trouvée
 *
 * @param commands: Tableau de commandes
 * @param cmd_count: Nombre de commandes
 * @param shell: Structure shell
 */
void	handle_command_not_found(t_cmd **commands, int cmd_count,
			t_shell *shell)
{
	printf("minishell: : command not found\n");
	shell->exit_status = 127;
	free_partial_cmds(commands, cmd_count);
}
