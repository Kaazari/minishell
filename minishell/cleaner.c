#include "minishell.h"

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
