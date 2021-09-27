NAME        = minishell

SRCS        = $(wildcard *.c)
OBJS        = $(SRCS:.c=.o)
LIBFT       = libft/libft.a
CC          = gcc  
FLAGS		= -Wall -Werror -Wextra
RM          = rm -rf

LDFLAGS="-L/Users/$$(whoami)/.brew/opt/readline/lib"
CPPFLAGS="-I/Users/$$(whoami)/.brew/opt/readline/include"

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC)  $(FLAGS) $(OBJS) -o $(NAME) $(LIBFT) -lreadline $(LDFLAGS) $(CPPFLAGS)

$(LIBFT):
	@make all -C ./libft --silent

clean:
	@$(RM) $(OBJS)
	@make clean -C ./libft --silent

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C ./libft --silent

re:            fclean all

.PHONY:        all clean fclean re
