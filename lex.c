#include "minishell.h"

simple_com	*init_simple_com(simple_com *s)
{
	s = malloc(sizeof(simple_com));
	if (!s)
		error_exit(1);
	s->command = NULL;
	s->option = NULL;
	s->arg = NULL;
	s->infile = NULL;
	s->outfile = NULL;
	s->rrfile = NULL;
	return (s);
}

char	*token(char *str, char **arg)
{
	int			len;
	int			i;
	int			start;

	i = 0;
	len = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	start = i;
	while (str[i] && !ft_isspace(str[i]))
	{
		i++;
		len++;
	}
	*arg = ft_substr(str, start, len);
	return (str + i);
}

simple_com	*fill_struct(char *str)
{
	simple_com	*s;

	s = init_simple_com(s);
	str = token(str, &s->command);
	str = token(str, &s->option);
	str = token(str, &s->arg);
	return (s);
}

simple_com	*split_pipes(char *str)
{
	char 		**tab;
	simple_com	*arr;
	int			i;

	tab = ft_split(str, '|');
	arr = malloc(sizeof(simple_com) * (pipe_count(str) + 1));
	if (!arr)
		exit(1);
	i = 0;
	while (i < pipe_count(str))
	{
		arr[i] = *fill_struct(tab[i]);
		++i;
	}
	arr[i] = (simple_com) {.command = NULL, .option = NULL, .arg = NULL};
	return (arr);
}

int	main(int argc, char **argv)
{
	simple_com *s;

	s = split_pipes(argv[1]);
	while (s->command)
	{
		printf("%s\n", s->command);
		printf("%s\n", s->option);
		printf("%s\n", s->arg);
		printf("\n");
		++s;
	}
	return (0);
}