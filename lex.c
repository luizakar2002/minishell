#include "minishell.h"

// simple_com	*init_simple_com(simple_com *s, char *str)
// {
// 	s = malloc(sizeof(simple_com));
// 	if (!s)
// 		error_exit(1);
// 	s->command = NULL;
// 	s->option = NULL;
// 	s->arg = NULL;
// 	s->infile = malloc(sizeof(char *) * char_count(str, '<'));
// 	s->outfile = malloc(sizeof(char *) * char_count(str, '>'));
// 	s->errfile = malloc(sizeof(char *) * str_count(str, "2>"));
// 	if ((!s->infile || !s->outfile || !s->errfile) && char_count(str, '<') && char_count(str, '>') && str_count(str, "2>"))
// 		error_exit(1);
// 	fill_null(s->infile, char_count(str, '<'));
// 	fill_null(s->outfile, char_count(str, '>'));
// 	fill_null(s->errfile, str_count(str, "2>"));
// 	return (s);
// }


// >   "dsaf""faffa" e''cho -n     is not working
simple_com	*init_simple_com(simple_com *s, char *str)
{
	s = malloc(sizeof(simple_com));
	if (!s)
		error_exit(1);
	s->command = NULL;
	s->option = NULL;
	s->arg = NULL;
	s->infile = malloc(sizeof(char *) * (char_count(str, '<') + 1));
	fill_null(s->infile, (char_count(str, '<') + 1));
	s->outfile = malloc(sizeof(char *) * (char_count(str, '>') + 1));
	fill_null(s->outfile, (char_count(str, '>') + 1));
	s->errfile = malloc(sizeof(char *) * (str_count(str, "2>") + 1));
	fill_null(s->errfile, (str_count(str, "2>") + 1));
	if (!s->outfile || !s->outfile || !s->errfile)
		error_exit(1);
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
	if (str[i] == '<' || str[i] == '>' || (str[i] == '2' && str[i+1] == '>'))
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
		s->arg = token; // ft_join
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
	arr = malloc(sizeof(simple_com) * (char_count(str, '|') + 2));
	if (!arr)
		exit(1);
	i = 0;
	while (i < (char_count(str, '|') + 1))
	{
		arr[i] = *fill_struct(tab[i]);
		++i;
	}
	//i++;
	arr[i] = (simple_com) {.command = NULL, .option = NULL, .arg = NULL};
	// if (open_files())
	// 	exit(2);
	return (arr);
}

int	main(int ac, char **av, char **env)
{
	simple_com *s;
	int			i;
	int			n;
	char		*str;
	pid_t		pid;
	pid_t		wpid;
	int			status;

	str = readline(NULL);
	n = char_count(str, '|') + 1;
	s = split_pipes(str);
	exec(s, n);
	//open files
	// while (pid != 0)
	// 	pid = fork();
	// 	if (pid == 0)
	// 		func()
	// 	if(pid != 0) 
	// 		waitpid(pid);
	// exec(s, n);
	// char *const parmList[] = {"ls", "-l", "-a", (char *)0};
    // char *const envParms[2] = {"PATH=/bin", (char*)0};
	// char *args[] = {s->command, "-l", (char *)0};
	// char *env_args[] = {"PATH=/bin", (char*)0};
	// pid = fork();
	// if (pid == 0)
	// {
	// 	execve(args[0], args, env_args);
	// 	printf("ERROR\n");
	// 	// if (execve(parmList[0], parmList, envParms) == -1)
	// 	// 	perror("lsh");
	// 	// exit(EXIT_FAILURE);
	// }
	// else
	// {
	// 	wait(NULL);
	// 	printf("success\n");
	// 	// do
	// 	// {
	// 	// 	wpid = waitpid(pid, &status, WUNTRACED);
	// 	// }
	// 	// while(!WIFEXITED(status) && !WIFSIGNALED(status));
	//}
	return (0);
}