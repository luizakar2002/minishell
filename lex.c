#include "minishell.h"

simple_com	*init_simple_com(simple_com *s, char *str, char **env)
{
	s = malloc(sizeof(simple_com));
	if (!s)
		error_exit(1);
	s->command = NULL;
	s->option = NULL;
	s->arg = NULL;
	s->infile = 0;
	s->outfile = 1;
	s->errfile = 2;
	return (s);
}

char	*token(char *str, simple_com *s)
{
	int			len;
	int			i;
	int			j;
	int			start;
	char		*token;
	static int	c;
	char		ch;
	char		redir;

	i = 0;
	j = 0;
	len = 0;
	redir = '.';
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '<' || str[i] == '>' || (str[i] == '2' && str[i+1] == '>') || (str[i] == '>' && str[i+1] == '>'))
	{
		redir = str[i];
		i++;
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i] == '"' || str[i] == '\'')
		{
			ch = str[i];
			i++;
			start = i;
			while (str[i] && str[i] != ch)
			{
				i++;
				len++;
			}
			i++;
		}
		else
		{
			start = i;
			while (str[i] && !ft_isspace(str[i]))
			{
				i++;
				len++;
			}
		}
	}
	else
	{
		start = i;
		while (str[i] && !ft_isspace(str[i]))
		{
			i++;
			len++;
		}
	}
	token = ft_substr(str, start, len);
	if (redir != '.')
		token = add_front(token, redir);
	token = remove_quote(token);
	if (check(token, s) == 1)
		s->command = token;
	else if (check(token, s) == 2)
		s->option = token;
	else if (check(token, s) == 3)
		s->arg = create_arg(s, token);
	else if (check(token, s) == 4)
		fill_fd(&s->infile, token + 1, 4);
	else if (check(token, s) == 5)
		fill_fd(&s->outfile, token + 1, 5);
	else if (check(token, s) == 6)
		fill_fd(&s->errfile, token + 2, 6);
	else if (check(token, s) == 7)
		fill_fd(&s->outfile, token + 2, 7);
	return (str + i);
}

simple_com	*fill_struct(char *str, char **env)
{
	simple_com	*s;

	s = init_simple_com(s, str, env);
	while (*str)
		str = token(str, s);
	return (s);
}

simple_com	*split_pipes(char *str, char **env)
{
	char 		**tab;
	simple_com	*arr;
	int			i;

	tab = ft_split(str, '|');
	arr = malloc(sizeof(simple_com) * (char_count(str, '|') + 2));
	if (!arr)
		exit(1);
	i = 0;
	while (i < (char_count(str, '|') + 1))
	{
		arr[i] = *fill_struct(tab[i], env);
		++i;
	}
	//i++;
	arr[i] = (simple_com) {.command = NULL, .option = NULL, .arg = NULL};
	// if (open_files())
	// 	exit(2);
	return (arr);
}

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
		ft_putstr_fd("minishell >   \b\b \nminishell > ", 1); 
	if (sig == SIGQUIT)
		ft_putstr_fd("minishell >   \b\b
		
		
		", 1);
}
// void	main_f()
// {
// 	simple_com *s;
// 	int			n;
// 	char		*str;

// 	signal(SIGINT, &handle_sigint);
// 	signal(SIGQUIT, &handle_sigint);
// 	str = readline("minishell > ");
// 	// if (!str)
// 	// 	str = readline("minishell > ");
// 	add_history(str);
// 	if (!ft_strncmp(str, "exit", 4))
// 		return ;
// 	n = char_count(str, '|') + 1;
// 	s = split_pipes(str, myenv);
// 	// print(s);
// 	exec(s, n);
// 	free(str);
// 	return ;
// }

int	main(int ac, char **av, char **env)
{
	simple_com *s;
	int			n;
	char		*str;

	myenv = env;
	while (1)
	{
		signal(SIGINT, &handle_sigint);
		signal(SIGQUIT, &handle_sigint);
		str = readline("minishell > ");
		if (!str)
			str = readline("minishell > ");
		add_history(str);
		if (!ft_strncmp(str, "exit", 4))
			break ;
		n = char_count(str, '|') + 1;
		s = split_pipes(str, env);
		// print(s);
		exec(s, n);
		free(str);
	}
	return (0);
}