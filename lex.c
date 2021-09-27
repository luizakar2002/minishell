#include "minishell.h"

simple_com	*init_simple_com(simple_com *s)
{
	s = malloc(sizeof(simple_com));
	if (!s)
		error_exit(5);
	s->command = NULL;
	s->option = NULL;
	s->arg = NULL;
	s->infile = 0;
	s->outfile = 1;
	s->dlm = NULL;
	s->herdoc = 0;
	return (s);
}

void	token(char *str, simple_com *s)
{
	int			len;
	int			i;
	int			j;

	i = 0;
	j = 0;
	len = 0;
	if (check(str, s) == 1)
		s->command = str;
	else if (check(str, s) == 8)
		s->command = str + 1;
	else if (check(str, s) == 9)
		s->arg = create_arg(s->arg, str + 1, 1);
	else if (check(str, s) == 2)
		s->option = str; 
	else if (check(str, s) == 3)
		s->arg = create_arg(s->arg, str, 1);
	else if (check(str, s) == 4)
		fill_fd(&s->infile, str + 1, 4);
	else if (check(str, s) == 5)
		fill_fd(&s->outfile, str + 1, 5);
	else if (check(str, s) == 6)
	{
		s->herdoc = 1;
		s->dlm = create_arg(s->dlm, str, 0);
	}
	else if (check(str, s) == 7)
		fill_fd(&s->outfile, str + 2, 7);
}

char	**divide(char *s)
{
	int		i;
	int		j;
	int		si;
	int		d;
	int		len;	
	int		word_cnt;
	char	**words;
	char	**w;
	char	*redi;

	w = NULL;
	s = ft_strtrim(s, " ");
	word_cnt = char_count(s, ' ');
	if (!s || !(words = (char **)malloc(sizeof(char *) * (word_cnt + 2))))
		error_exit(5);
	i = 0;
	j = 0;
	d = -1;
	si = -1;
	len = 0;
	redi = NULL;
	while (s[i])
	{
		if (s[i] && s[i] == '"')
		{
			d *= -1;
			len++;
		}
		else if (s[i] && s[i] == '\'')
		{
			si *= -1;
			len++;
		}
		else if (s[i] && ft_isspace(s[i]))
		{
			if (d == -1 && si == -1)
			{
				if (redi)
				{
					words[j++] = ft_strjoin(redi, ft_substr(s, i - len, len), 0);
					redi = NULL;
				}
				else
					words[j++] = ft_substr(s, i - len, len);	
				len = 0;
				while (s[i] && ft_isspace(s[i]))
					i++;
				continue ;
			}
			else
				len++;
		}
		else if (redir(s + i) && d == -1 && si == -1)
		{
			if (len != 0)
				words[j++] = ft_substr(s, i - len, len);
			redi = redir(s + i);
			i += ft_strlen(redi);
			while (s[i] && ft_isspace(s[i]))
				i++;
			i--;
			len = 0;
		}
		else
			len++;
		i++;
	}
	if (redi)
		words[j++] = ft_strjoin(redi, ft_substr(s, i - len, len), 0);
	else
		words[j++] = ft_substr(s, i - len, len);
	words[j] = 0;
	if (!s || !(w = (char **)malloc(sizeof(char *) * (j + 1))))
		error_exit(5);
	w[j] = 0;
	i = 0;
	while (i < j)
	{
		w[i] = ft_strdup(words[i]);
		i++;
	}
	free_2d(words);
	return (w);
}

simple_com	*fill_struct(char *str, t_env *env)
{
	simple_com	*s;
	char		**div;
	int			i;
	char		*ss;

	s = NULL;
	ss = NULL;
	s = init_simple_com(s);
	div = divide(str);
	if (!div)
		return (NULL);
	i = -1;
	while (div[++i])
	{
		token(remove_quote(div[i], env), s);
	}
	free(div);
	return (s);
}

simple_com	*split_pipes(char *str, t_env *env)
{
	char 		**tab;
	simple_com	*arr;
	int			i;
	simple_com	*temp;

	tab = ft_split(str, '|');
	if (!tab[0])
		return (NULL);
	arr = malloc(sizeof(simple_com) * (char_count(str, '|') + 2));
	if (!arr)
		error_exit(5);
	i = 0;
	while (i < (char_count(str, '|') + 1))
	{
		if (!is_valid(tab[i])) // in is valid check redirection syntax errors
			return (NULL);
		temp = fill_struct(tab[i], env);
		if (!temp)
			return (NULL);
		arr[i] = *temp;
		if (!check_com(arr[i].command))
			return (NULL);
		++i;
	}
	arr[i] = (simple_com) {.command = NULL, .option = NULL, .arg = NULL};
	return (arr);
}

void	handle_sigint(int sig)
{
	struct termios term;

	tcgetattr(0, &term);
	term.c_lflag &= ~ECHO;
	term.c_lflag &= ~ECHOCTL;
	term.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &term);
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
