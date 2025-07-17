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
			$(SRC_DIR)/core/shell_utils.c

PARSING_SRCS = $(SRC_DIR)/parsing/tokenize.c \
			   $(SRC_DIR)/parsing/tokenize_utils.c \
			   $(SRC_DIR)/parsing/tokenize_commands.c

EXECUTION_SRCS = $(SRC_DIR)/execution/externals.c \
				 $(SRC_DIR)/execution/redirections.c \
				 $(SRC_DIR)/execution/redirections_utils.c

BUILTINS_SRCS = $(SRC_DIR)/builtins/builtin.c

UTILS_SRCS = $(SRC_DIR)/utils/utils.c \
			 $(SRC_DIR)/utils/utils2.c

MEMORY_SRCS = $(SRC_DIR)/memory/cleaner.c

PIPEX_SRCS = $(PIPEX_DIR)/pipes.c \
			 $(PIPEX_DIR)/utils.c

# All source files
SRCS = $(CORE_SRCS) $(PARSING_SRCS) $(EXECUTION_SRCS) \
	   $(BUILTINS_SRCS) $(UTILS_SRCS) $(MEMORY_SRCS) $(PIPEX_SRCS)

OBJS = $(SRCS:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I$(PIPEX_DIR)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
