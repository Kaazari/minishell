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

// void free_split(char **split)
// {
// 	int i;
// 	if (!split)
// 		return ;
// 	i = 0;
// 	while (split[i])
// 	{
// 		free(split[i]);
// 		split[i] = NULL;
// 		i++;
// 	}
// 	free(split);
// }

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	if (cmd->redirs)
	{
		i = 0;
		while (i < cmd->redir_count)
			free(cmd->redirs[i++].file);
		free(cmd->redirs);
	}
	free(cmd);
}

void	free_cmds(t_cmd **cmds)
{
	int i = 0;
	if (!cmds)
		return;
	while (cmds[i])
	{
		free_cmd(cmds[i]);
		i++;
	}
	free(cmds);
}

void free_cmds_array(t_cmd ***cmds_array, int count)
{
    int i;
    if (!cmds_array)
        return;
    for (i = 0; i < count; i++)
        free_cmds(cmds_array[i]);
    free(cmds_array);
}

void free_partial_cmds(t_cmd **cmds, int count)
{
    int i;
    if (!cmds)
        return;
    for (i = 0; i < count; i++)
        free_cmd(cmds[i]);
    free(cmds);
}
