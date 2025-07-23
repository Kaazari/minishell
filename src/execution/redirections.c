/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redirections(t_cmd *cmd, t_shell *shell)
{
	int	i;

	if (!cmd || !cmd->redirs)
		return ;
	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_OUT
			|| cmd->redirs[i].type == REDIR_APPEND)
		{
			handle_output_redirection(cmd, i);
		}
		else if (cmd->redirs[i].type == REDIR_IN)
		{
			handle_input_redirection(cmd, i);
		}
		else if (cmd->redirs[i].type == REDIR_HEREDOC)
		{
			handle_heredoc(cmd, i, shell);
		}
		i++;
	}
}
