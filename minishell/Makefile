###############################################
#	SOURCES

SRC		= minishell.c builtin.c utils.c tokenize.c cleaner.c externals.c \

OBJ		= $(SRC:.c=.o)

CC		= cc

CFLAGS	= -Wall -Wextra -Werror

RLFLAGS	= -lreadline

LIBFT	= ./libft/libft.a

RM		= rm -f

NAME	= minishell

###############################################
#	MAKE

%.o:	%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -g3

all:	$(NAME)

$(LIBFT):
	@make -C ./libft

$(NAME):	$(OBJ) $(LIBFT)
	@$(CC) $(OBJ) $(LIBFT) $(RLFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJ)
	@make clean -C ./libft

fclean:		clean
	$(RM) $(NAME)
	@make fclean -C ./libft

re:			fclean all

.PHONY:		all clean fclean re
