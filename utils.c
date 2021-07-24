#include "minishell.h"

int	pipe_count(char *str)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
		if (str[i++] == '|')
			++count;
	return (++count);
}

void	error_exit(int error)
{
	if (error == 1)
	{
		printf("No memory to allocate\n");
		exit(1);
	}
}