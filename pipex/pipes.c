/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/19 18:30:00 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "pipex.h"

/**
 * Initialise la structure pipex pour gérer les pipes
 *
 * Cette fonction :
 * - Définit le nombre de commandes et de pipes
 * - Alloue le tableau de descripteurs de fichiers pour les pipes
 * - Crée les pipes nécessaires
 * - Gère le cas où il n'y a qu'une seule commande (pas de pipes)
 *
 * @param pipex: Structure pipex à initialiser
 * @param cmd_count: Nombre de commandes dans le pipeline
 *
 * Note: Si l'allocation ou la création des pipes échoue, pipe_fds est mis à NULL
 */
void	init_pipes(t_pipex *pipex, int cmd_count)
{
	pipex->cmd_count = cmd_count;
	pipex->pipe_count = cmd_count - 1;
	if (pipex->pipe_count <= 0)
	{
		pipex->pipe_fds = NULL;
		return ;
	}
	pipex->pipe_fds = allocate_pipe_fds(pipex->pipe_count);
	if (!pipex->pipe_fds)
		return ;
	if (!create_pipes(pipex->pipe_fds, pipex->pipe_count))
		pipex->pipe_fds = NULL;
}

/**
 * Ferme tous les descripteurs de fichiers des pipes et libère la mémoire
 *
 * Cette fonction :
 * - Ferme tous les descripteurs de fichiers des pipes
 * - Libère le tableau de descripteurs
 * - Met le pointeur à NULL
 *
 * @param pipex: Structure pipex contenant les descripteurs à fermer
 *
 * Note: Cette fonction vérifie l'existence de pipe_fds avant de procéder
 */
void	close_pipes(t_pipex *pipex)
{
	int	i;

	if (!pipex->pipe_fds)
		return ;
	i = 0;
	while (i < 2 * pipex->pipe_count)
	{
		close(pipex->pipe_fds[i]);
		i++;
	}
	free(pipex->pipe_fds);
	pipex->pipe_fds = NULL;
}

/**
 * Attend la terminaison de tous les processus enfants
 *
 * Cette fonction :
 * - Attend chaque processus enfant avec waitpid
 * - Récupère le statut de sortie de chaque processus
 *
 * @param pipex: Structure pipex contenant le nombre de commandes
 * @param child_pids: Tableau des PIDs des processus enfants
 *
 * Note: Cette fonction attend tous les processus dans l'ordre de création
 */
void	wait_children(t_pipex *pipex, pid_t *child_pids)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(child_pids[i], &status, 0);
		i++;
	}
}

/**
 * Exécute des commandes en parallèle avec des pipes
 *
 * Cette fonction implémente l'exécution de pipelines de commandes :
 * - Crée un processus enfant pour chaque commande
 * - Établit les pipes entre les processus
 * - Configure les redirections d'entrée/sortie pour chaque processus
 * - Gère les redirections (heredoc, <, >, >>) dans les processus enfants
 * - Attend la terminaison de tous les processus
 *
 * Gestion des pipes :
 * - Chaque processus lit depuis le pipe précédent (sauf le premier)
 * - Chaque processus écrit vers le pipe suivant (sauf le dernier)
 * - Les descripteurs de fichiers sont fermés dans le processus parent
 *
 * Gestion des redirections :
 * - Les heredocs sont prétraités et leurs descripteurs sont utilisés
 * - Les redirections d'entrée (<) remplacent STDIN
 * - Les redirections de sortie (> >>) remplacent STDOUT
 *
 * @param shell: Structure shell contenant l'état
 * @param commands: Tableau de commandes à exécuter
 * @param cmd_count: Nombre de commandes dans le pipeline
 *
 * Note: Cette fonction utilise un tableau de PIDs limité à 256 processus.
 * Les processus enfants se terminent avec exit() après exécution.
 */
void	execute_piped_commands(t_shell *shell, t_cmd **commands, int cmd_count)
{
	int pipe_fd[2];
	int prev_fd = -1;
	pid_t pids[256];
	int i;

	for (i = 0; i < cmd_count; i++)
	{
		if (i < cmd_count - 1)
		{
			if (pipe(pipe_fd) < 0)
			{
				perror("pipe");
				return;
			}
		}
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("fork");
			return;
		}
		if (pids[i] == 0)
		{
			setup_child_signals();
			if (i > 0)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (i < cmd_count - 1)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
			}
			if (commands[i] && commands[i]->redirs)
			{
				for (int j = 0; j < commands[i]->redir_count; j++)
				{
					if (commands[i]->redirs[j].type == REDIR_HEREDOC && commands[i]->redirs[j].is_heredoc_fd > 0)
					{
						dup2(commands[i]->redirs[j].is_heredoc_fd, STDIN_FILENO);
						close(commands[i]->redirs[j].is_heredoc_fd);
					}
					else if (commands[i]->redirs[j].type == REDIR_IN)
					{
						int fd = open(commands[i]->redirs[j].file, O_RDONLY);
						if (fd >= 0)
						{
							dup2(fd, STDIN_FILENO);
							close(fd);
						}
					}
					else if (commands[i]->redirs[j].type == REDIR_OUT || commands[i]->redirs[j].type == REDIR_APPEND)
					{
						int flags = O_WRONLY | O_CREAT;
						if (commands[i]->redirs[j].type == REDIR_APPEND)
							flags |= O_APPEND;
						else
							flags |= O_TRUNC;
						int fd = open(commands[i]->redirs[j].file, flags, 0644);
						if (fd >= 0)
						{
							dup2(fd, STDOUT_FILENO);
							close(fd);
						}
					}
				}
			}
			shell->cmd = commands[i];
			if (shell->pipex)
			{
				shell->pipex->cmd_count = cmd_count;
				shell->pipex->pipe_count = cmd_count - 1;
			}
			execute_shell_command(commands[i]->args, shell, cmd_count);
			exit(shell->exit_status);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (i < cmd_count - 1)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
	}
	close_parent_heredoc_fds(commands, cmd_count);
	if (prev_fd != -1)
		close(prev_fd);
	for (i = 0; i < cmd_count; i++)
		waitpid(pids[i], NULL, 0);
}
