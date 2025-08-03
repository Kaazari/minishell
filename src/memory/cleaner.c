/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:31:27 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirs)
	{
		i = 0;
		while (i < cmd->redir_count)
		{
			free(cmd->redirs[i].file);
			i++;
		}
		free(cmd->redirs);
	}
	free(cmd);
}

void	free_cmds(t_cmd **cmds)
{
	int	i;

	i = 0;
	if (!cmds)
		return ;
	while (cmds[i])
	{
		free_cmd(cmds[i]);
		i++;
	}
	free(cmds);
}

void	free_partial_cmds(t_cmd **cmds, int count)
{
	int	i;

	i = 0;
	if (!cmds)
		return ;
	while (i < count && cmds[i])
	{
		free_cmd(cmds[i]);
		i++;
	}
	free(cmds);
}
