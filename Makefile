NAME	= minishell

SRCS	= $(wildcard *.c)
OBJS	= $(SRCS:%.c=%.o)

GCC		= gcc -Wall -Werror -Wextra
RM		= rm -rf
LIBFT	= libft/libft.a

all:	$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
			$(GCC) $(OBJS) -o $(NAME) $(LIBFT)

$(LIBFT):
			make all -C ./libft

clean:
			$(RM) $(OBJS)
			make clean -C ./libft

fclean:		clean
			$(RM) $(NAME)
			make fclean -C ./libft

re:			fclean all

.PHONY:		all clean fclean re