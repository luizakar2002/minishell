#include "minishell.h"

simple_com	*init_simple_com(simple_com *s, char *str)
{
	s = malloc(sizeof(simple_com));
	if (!s)
		error_exit(1);
	s->command = NULL;
	s->option = NULL;
	s->arg = NULL;
	s->infile = malloc(sizeof(char *) * char_count(str, '<'));
	fill_null(s->infile, char_count(str, '<'));
	s->outfile = malloc(sizeof(char *) * char_count(str, '>'));
	fill_null(s->outfile, char_count(str, '>'));
	s->errfile = malloc(sizeof(char *) * err_count(str));
	fill_null(s->errfile, err_count(str));
	if (!s->outfile || !s->outfile || !s->errfile)
		error_exit(1);
	return (s);
}

char	*token(char *str, simple_com *s)
{
	int			len;
	int			i;
	int			start;
	char		*token;
	static int	c;

	i = 0;
	len = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '\'')
	{
		printf("1\n");
		start = i + 1;
		while (str[i] && str[i] != '\'')
		{
			i++;
			len++;
		}
	}
	else if (str[i] == '"')
	{
		printf("2\n");
		start = i + 1;
		while (str[i] && str[i] != '"')
		{
			i++;
			len++;
		}
	}
	else if (str[i] == '<' || str[i] == '>' || (str[i] == '2' && str[i+1] == '>'))
	{
		printf("3\n");
		start = i;
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i] == '"')
		while (str[i] && )
		{
			i++;
			len++;
		}
	}
	token = ft_substr(str, start, len);
	// printf("str %s\n", token);
	if (check(token, s) == 1)
		s->command = token;
	else if (check(token, s) == 2)
		s->option = token;
	else if (check(token, s) == 3)
		s->arg = token;
	else if (check(token, s) == 4)
		fill_matrix(s->infile, token + 1);
	else if (check(token, s) == 5)
		fill_matrix(s->outfile, token + 1);
	else if (check(token, s) == 6)
		fill_matrix(s->errfile, token + 2);
	return (str + i);
}

simple_com	*fill_struct(char *str)
{
	simple_com	*s;

	s = init_simple_com(s, str);
	while (*str)
		str = token(str, s);
	return (s);
}

simple_com	*split_pipes(char *str)
{
	char 		**tab;
	simple_com	*arr;
	int			i;

	tab = ft_split(str, '|');
	arr = malloc(sizeof(simple_com) * (char_count(str, '|') + 1));
	if (!arr)
		exit(1);
	i = 0;
	while (i < char_count(str, '|'))
	{
		arr[i] = *fill_struct(tab[i]);
		++i;
	}
	arr[i] = (simple_com) {.command = NULL, .option = NULL, .arg = NULL};
	// if (open_files())
	// 	exit(2);
	return (arr);
}

int	main(void)
{
	simple_com *s;
	int			i;
	char		*str;

	str = readline(NULL);
	s = split_pipes(str);
	while (s->command)
	{
		printf("%s\n", s->command);
		printf("%s\n", s->option);
		printf("%s\n", s->arg);
		i = 0;
		while (s->infile[i])
			printf("%s\n", s->infile[i++]);
		i = 0;
		while (s->outfile[i])
			printf("%s\n", s->outfile[i++]);
		i = 0;
		while (s->errfile[i])
			printf("%s\n", s->errfile[i++]);
		printf("\n");
		++s;
	}
	return (0);
}