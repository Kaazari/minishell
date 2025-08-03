# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kclaudan <kclaudan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/01 00:00:00 by anonymous         #+#    #+#              #
#    Updated: 2025/07/17 16:28:34 by kclaudan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

# Directories
SRC_DIR = src
INCLUDE_DIR = include
LIBFT_DIR = ./libft
PIPEX_DIR = ./pipex

# Source files organized by modules
CORE_SRCS = $(SRC_DIR)/core/minishell.c \
			$(SRC_DIR)/core/shell_utils.c \
			$(SRC_DIR)/core/shell_utils_helpers.c \
			$(SRC_DIR)/core/shell_cleanup.c \
			$(SRC_DIR)/core/shell_signals.c \
			$(SRC_DIR)/core/shell_execution.c \
			$(SRC_DIR)/core/shell_execution_helpers.c \
			$(SRC_DIR)/core/shell_commands.c \
			$(SRC_DIR)/core/shell_commands_helpers.c

PARSING_SRCS = $(SRC_DIR)/parsing/tokenize_utils.c \
			   $(SRC_DIR)/parsing/tokenize_input.c \
			   $(SRC_DIR)/parsing/tokenize_input_helpers.c \
			   $(SRC_DIR)/parsing/tokenize_input_utils.c \
			   $(SRC_DIR)/parsing/tokenize_input_iteration.c \
			   $(SRC_DIR)/parsing/tokenize_input_quotes.c \
			   $(SRC_DIR)/parsing/tokenize_syntax.c \
			   $(SRC_DIR)/parsing/tokenize_commands.c \
			   $(SRC_DIR)/parsing/tokenize_commands_helpers.c \
			   $(SRC_DIR)/parsing/tokenize_commands_utils.c \
			   $(SRC_DIR)/parsing/tokenize_helpers.c \
			   $(SRC_DIR)/parsing/tokenize_helpers_utils.c \
			   $(SRC_DIR)/parsing/tokenize_env.c \
			   $(SRC_DIR)/parsing/tokenize_cmd.c \
			   $(SRC_DIR)/parsing/tokenize_cmd_helpers.c \
			   $(SRC_DIR)/parsing/tokenize_cmd_utils.c \
			   $(SRC_DIR)/parsing/tokenize_redirection.c

EXECUTION_SRCS = $(SRC_DIR)/execution/externals.c \
				$(SRC_DIR)/execution/externals_helpers.c \
				$(SRC_DIR)/execution/externals_utils.c \
				$(SRC_DIR)/execution/redirections.c \
				$(SRC_DIR)/execution/redirections_helpers.c \
				$(SRC_DIR)/execution/redirections_heredoc.c \
				$(SRC_DIR)/execution/redirections_heredoc_helpers.c \
				$(SRC_DIR)/execution/redirections_heredoc_utils.c \
				$(SRC_DIR)/execution/redirections_signals.c \
				$(SRC_DIR)/execution/redirections_utils.c

BUILTINS_SRCS = $(SRC_DIR)/builtins/builtin_navigation_helpers.c \
				$(SRC_DIR)/builtins/builtin_navigation_paths.c \
				$(SRC_DIR)/builtins/builtin_navigation_expand.c \
				$(SRC_DIR)/builtins/builtin_env_utils.c \
				$(SRC_DIR)/builtins/builtin_unset_utils.c \
				$(SRC_DIR)/builtins/builtin_utils.c

UTILS_SRCS = $(SRC_DIR)/utils/utils.c \
			 $(SRC_DIR)/utils/utils_helpers.c \
			 $(SRC_DIR)/utils/env_utils.c \
			 $(SRC_DIR)/utils/memory_utils.c

MEMORY_SRCS = $(SRC_DIR)/memory/cleaner.c \
			  $(SRC_DIR)/memory/cleanup_utils.c \
			  $(SRC_DIR)/memory/cleanup_utils_helpers.c

PIPEX_SRCS = $(PIPEX_DIR)/pipes.c \
			$(PIPEX_DIR)/pipes_helpers.c \
			$(PIPEX_DIR)/pipes_redirections.c \
			$(PIPEX_DIR)/pipes_execution.c \
			$(PIPEX_DIR)/pipes_child.c \
			$(PIPEX_DIR)/pipes_setup.c \
			$(PIPEX_DIR)/pipes_utils.c \
			$(PIPEX_DIR)/utils.c

# All source files
SRCS = $(CORE_SRCS) $(PARSING_SRCS) $(EXECUTION_SRCS) \
	   $(BUILTINS_SRCS) $(UTILS_SRCS) $(MEMORY_SRCS) $(PIPEX_SRCS)

OBJS = $(SRCS:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -g3 -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(PIPEX_DIR)

# Règle pour compiler les .c en .o sans affichage
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline
	@echo "minishell ready"

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
