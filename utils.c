#include "minishell.h"

int	char_count(char *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
		if (str[i++] == c)
			++count;
	return (++count);
}

int	err_count(char *str)
{
	int	count;
	int	i;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] == '2' && str[i + 1] == '>')
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

int	check(char *str, simple_com *s)
{
	if (str && str[0] == '<')
		return (4);
	else if (str && str[0] == '>')
		return (5);
	else if (str && str[0] == '2' && str[1] == '>')
		return (6);
	else if (str && str[0] == '-' && str[1] == 'n')
		return (2);
	else if (s->command == NULL)
		return (1);
	else
		return (3);
}

void	fill_null(char **arr, int c)
{
	while (c-- > 0)
		arr[c] = NULL;
}

void	fill_matrix(char **arr, char *str)
{
	int i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	arr[i] = str;
}
