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

int	preprocess_all_heredocs(t_shell *shell, t_cmd **commands, int cmd_count)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmd_count && commands[i])
	{
		if (has_heredoc(commands[i]))
		{
			j = 0;
			while (j < commands[i]->redir_count)
			{
				if (commands[i]->redirs[j].type == REDIR_HEREDOC)
				{
					handle_heredoc(commands[i], j, shell);
					if (shell->state == 1)
						return (-1);
				}
				j++;
			}
		}
		i++;
	}
	return (0);
}





static void	handle_heredoc_redir(t_cmd *cmd, int i)
{
	if (cmd->redirs[i].type == REDIR_HEREDOC)
	{
		if (cmd->redirs[i].is_heredoc_fd > 0)
		{
			dup2(cmd->redirs[i].is_heredoc_fd, STDIN_FILENO);
			close(cmd->redirs[i].is_heredoc_fd);
			cmd->redirs[i].is_heredoc_fd = -1;
		}
	}
}

void	handle_redirections(t_cmd *cmd, t_shell *shell)
{
	int	i;

	(void)shell;
	if (!cmd || !cmd->redirs)
		return ;
	// Traiter seulement la dernière redirection de sortie
	i = cmd->redir_count - 1;
	while (i >= 0)
	{
		if (cmd->redirs[i].type == REDIR_OUT || cmd->redirs[i].type == REDIR_APPEND)
		{
			handle_output_redirection(cmd, i);
			break ;
		}
		i--;
	}
	// Traiter seulement la dernière redirection d'entrée
	i = cmd->redir_count - 1;
	while (i >= 0)
	{
		if (cmd->redirs[i].type == REDIR_IN)
		{
			handle_input_redirection(cmd, i);
			break ;
		}
		i--;
	}
	// Traiter les heredocs dans l'ordre normal
	i = 0;
	while (i < cmd->redir_count)
	{
		handle_heredoc_redir(cmd, i);
		i++;
	}
}
