#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include "pipex/pipex.h"

# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

typedef struct s_redir
{
	int		type;
	char	*file;
	int		fd;
}	t_redir;

typedef struct s_cmd
{
	char		**args;
	t_redir		*redirs;
	int			redir_count;
	int			stdin_backup;
	int			stdout_backup;
}	t_cmd;

typedef struct s_shell
{
	char		**envp;
	char		**local_envp;
	t_cmd		*cmd;
	t_pipex		*pipex;
	int			state;
	int			exit_status;
}	t_shell;

typedef struct s_token_data
{
	char	*current_word;
	int		*word_pos;
	int		*word_count;
	char	**words;
}	t_token_data;

void	execute_shell_command(char **args, t_shell *shell);
void	execute_external_commands(char **args, char **envp);
void	signal_handler(int signo);
t_cmd	*create_cmd(void);
void	free_cmd(t_cmd *cmd);
void	free_args(char **args);
char	**tokenize_words(char *input);
t_cmd	**tokenize_piped_commands(char *input, int *cmd_count);
char	*find_command_in_path(char *command);
char	*check_absolute_path(char *command);
char	*search_in_path(char *command, char **paths);
void	print_export_format(char **envp);
void	add_or_update_env(char *var, t_shell *shell);
int		update_existing_env_var(char *var, t_shell *shell);
void	add_new_env_var(char *var, t_shell *shell);
void	export_existing_var(char *var, t_shell *shell);
void	handle_parent_process(pid_t pid, int *status, char *path);
char	*validate_and_get_path(char **args);
void	handle_path_not_found(char *command);
void	setup_child_signals(void);
void	main_shell_loop(t_shell *shell);

int		builtin_cd(char **args);
int		builtin_pwd(char **args);
int		builtin_export(char **args, t_shell *shell);
int		builtin_unset(char **args, t_shell *shell);
int		builtin_colon(char **args);
int		builtin_env(char **args, t_shell *shell);
void	builtin_exit(char **args);
int		builtin_echo(char **args);

void	handle_redirections(t_cmd *cmd);
void	handle_output_redirection(t_cmd *cmd, int i);
void	handle_input_redirection(t_cmd *cmd, int i);
void	restore_redirections(t_cmd *cmd);
void	restore_output_redirection(t_cmd *cmd, int i);
void	restore_input_redirection(t_cmd *cmd, int i);

void	execute_piped_commands(t_shell *shell, t_cmd **commands, int cmd_count);

#endif
