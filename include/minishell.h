/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#             */
/*   Updated: 2025/07/17 16:30:30 by kclaudan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* System includes */
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <ctype.h>

/* Project includes */
# include "../libft/libft.h"
# include "../pipex/pipex.h"

/* Constants */
# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

/* Global variable for signal handling only */
extern volatile sig_atomic_t	g_signal;

/* Structures */
typedef struct s_redir
{
	int			type;
	char		*file;
	int			fd;
}				t_redir;

typedef struct s_cmd
{
	char		**args;
	t_redir		*redirs;
	int			redir_count;
	int			stdin_backup;
	int			stdout_backup;
}				t_cmd;

typedef struct s_shell
{
	char		**envp;
	char		**local_envp;
	t_cmd		*cmd;
	t_pipex		*pipex;
	int			state;
	int			exit_status;
	t_cmd		**current_commands;
	int			current_cmd_count;
}				t_shell;

typedef struct s_token_data
{
	char		*current_word;
	int			*word_pos;
	int			*word_count;
	char		**words;
}				t_token_data;

typedef struct s_var_expansion
{
	char	*input;
	int		i;
	char	*word;
	int		*pos;
}				t_var_expansion;

typedef struct s_cmd_process
{
	t_cmd	**commands;
	char	**words;
	int		*cmd_idx;
	int		*start;
	int		*cmd_count;
}				t_cmd_process;

typedef struct s_tokenize_init
{
	char	**words;
	char	*current_word;
	int		*word_count;
	int		*word_pos;
}				t_tokenize_init;

/* Function prototypes - organized by modules */

/* Core functions */
void		execute_shell_command(char **args, t_shell *shell);
void		clean_exit(t_shell *shell, int status);
void		signal_handler(int signo);
void		setup_signals(void);
void		main_shell_loop(t_shell *shell);
int			process_empty_input(char *input);
void		handle_single_command(t_shell *shell, t_cmd **commands);
void		handle_piped_commands(t_shell *shell, t_cmd **commands, int count);
void		process_command(t_shell *shell, char *input);

/* Parsing functions */
void		free_args(char **args);
void		process_command_words(t_cmd *cmd, char **words);
t_cmd		**tokenize_piped_commands(char *input, int *cmd_count,
				t_shell *shell);
char		**create_cmd_words(char **words, int start, int end);
void		process_final_command(t_cmd_process *proc, int start, int i);
t_cmd		**init_commands_array(char **words);
char		**tokenize_words(char *input, t_shell *shell);
void		process_input_chars(char *input, t_token_data *data,
				char *quote_state, t_shell *shell);
void		handle_redirection_token(t_token_data *data, char *input, int *i);
void		handle_pipe_token(t_token_data *data, int *i);
void		handle_separator_token(t_token_data *data, int *i);
int			check_syntax_errors(char **words);
int			add_variable_to_word(t_var_expansion *var_exp, t_shell *shell);
void		add_char_to_word(char *word, int *pos, char c);
void		save_word(char **words, int *count, char *word, int len);
int			is_separator(char c);
int			is_quote(char c);
char		update_quote_state(char c, char current_state);
char		*get_env_value(char *var, char **envp);
t_cmd		*create_cmd(void);
void		add_redirection(t_cmd *cmd, int type, char *file);
int			get_redirection_type(char *token);
void free_partial_cmds(t_cmd **cmds, int count);

/* Execution functions */
void		execute_external_commands(char **args, char **envp, t_shell *shell);
void		handle_parent_process(pid_t pid, int *status, char *path,
				t_shell *shell);
char		*validate_and_get_path(char **args, t_shell *shell);
void		handle_path_not_found(char *command, t_shell *shell);
void		setup_child_signals(void);
void		handle_redirections(t_cmd *cmd, t_shell *shell);
void		handle_output_redirection(t_cmd *cmd, int i);
void		handle_input_redirection(t_cmd *cmd, int i);
void		handle_heredoc(t_cmd *cmd, int i, t_shell *shell);
void		setup_heredoc_signals(void);
void		restore_main_signals(void);
void		restore_redirections(t_cmd *cmd);
void		restore_output_redirection(t_cmd *cmd, int i);
void		restore_input_redirection(t_cmd *cmd, int i);
void		execute_piped_commands(t_shell *shell, t_cmd **commands,
				int cmd_count);
void		setup_child_pipes(t_shell *shell, int i, int cmd_count);
void		execute_child_command(t_shell *shell, t_cmd *command);
void		init_pipes(t_pipex *pipex, int cmd_count);
int			*allocate_pipe_fds(int pipe_count);
int			create_pipes(int *pipe_fds, int pipe_count);

/* Built-in functions */
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(char **args);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_colon(char **args);
int			builtin_env(char **args, t_shell *shell);
int		builtin_exit(char **args);
int			builtin_echo(char **args);

/* Utility functions */
char		*find_command_in_path(char *command);
char		*check_absolute_path(char *command);
char		*search_in_path(char *command, char **paths);
void		print_export_format(char **envp);
void		add_or_update_env(char *var, t_shell *shell);
int			update_existing_env_var(char *var, t_shell *shell);
void		add_new_env_var(char *var, t_shell *shell);
void		export_existing_var(char *var, t_shell *shell);

/* Memory management functions */
void		free_cmd(t_cmd *cmd);
void		free_cmds(t_cmd **cmds);
void		free_cmds_array(t_cmd ***cmds_array, int count);
char **dup_envp(char **envp);

#endif
