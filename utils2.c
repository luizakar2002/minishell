#include "minishell.h"

int	check_export(char *arg)
{
	int	i;
	
	if (ft_isalpha(arg[0]) || arg[0] == '_')
	{
		i = 1;
		while (arg[i] && arg[i] != '=')
		{
			if (ft_isalnum(arg[i]) || arg[i] == '_')
				++i;
			else
				return (0);
		}
		if (arg[i] && arg[i] == '=')
			return (2);
		return (1);
	}
	else
		return (0);
}

int	heredoc(simple_com *s, t_env *e)
{
	int 	i;
	int		j;
	int		k;
	int		n1;
	int		n;
	char	*str;
	pid_t 	pid; 

	i = 0;
	n = 0;
	k = 0;
	while (s[i].command || s[i].herdoc)
	{
		if (s[i].dlm)
			++n;
		if (s[i].herdoc)
			++k;
		i++;
	}
	if (k == 0)
		return (0);
	if (n == 0)
		return (2);
	int		fd[n][2];
	i = 0;
	n1 = 0;
	k = 0;
	while (i < n)
	{
		if (pipe(fd[i]) == -1)
			error_exit(6);
		i++;
	}
	i = 0;
	while (s[i].command || s[i].dlm)
	{
		if (s[i].dlm)
		{
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				j = 0;
				while (s[i].dlm[j])
				{
					if (s[i].dlm[j][0] == 3)
						k = 3;
					else
						k = 2;
					while (1)
					{
						str = readline("> ");
						if (!str || ft_strncmp(str, s[i].dlm[j] + k, ft_strlen(s[i].dlm[j] + k) + 1) == 0)
							break ;
						else
						{
							if (s[i].dlm[j + 1] == NULL)
							{
								if (k == 2)
									str = parse_heredoc(str, e);
								ft_putendl_fd(str, fd[n1][1]);
							}
						}
					}
					j++;
				}
				k = 0;
				while (k < n)
				{
					close(fd[k][0]);
					close(fd[k][1]);
					k++;
				}
				n1++;
				exit(0);
			}
			else
			{
				signal(SIGINT, SIG_IGN);
				waitpid(pid, &status, 0);
				s[i].infile = fd[n1][0];
				close(fd[n1][1]);
				if (WTERMSIG(status))
				{
					status = 1;
					return (1);
				}
				n1++;
			}
		}
		i++;
	}
	return (0);
}

char     *parse_heredoc(char *str, t_env *e)
{
    char    *s;
    int     len;
    int        qmark;
    int        i;
    char     *j;

    j = malloc(sizeof(char) * 2);
    if (!j)
        error_exit(5);
    j[1] = '\0';
    i = 0;
    s = malloc(2);
    qmark = -1;
    while (str[i])
    {
        if (str[i] && str[i] == '$')
        {
            len = 0;
            i++;
            qmark = i;
            while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '"')
            {
                len++;
                i++;
            }
            if (str[qmark] == '?')
            {
                i = qmark;
                i++;
                len = 1;
            }
            if (len == 0)
            {
                j[0] = str[i - 1];
                s = ft_strjoin(s, j, 1);
            }
            else
            {
                if (!get_env(ft_substr(str, i - len, len), e))
                    s = ft_strjoin(s, "", 1);
                else
                    s = ft_strjoin(s, get_env(ft_substr(str, i - len, len), e), 1);
                if (qmark != -1)
                    i--;
            }
        }
        else
        {
            j[0] = str[i];
            s = ft_strjoin(s, j, 1);
        }
        i++;
    }
    free(j);
    return (s);
}

int		is_valid(char *str)
{
    int i;

    i = -1;
    while (str && str[++i])
    {
        if (!ft_isspace(str[i]))
            return (1);
    }
    return (0);
}

void	free_coms(simple_com *s)
{
	int i;
	int	n;

	i = 0;
	n = 0;
	while (s[n].command || s[n].herdoc)
		++n;
	while(i < n)
	{
		if (s[i].command)
			free(s[i].command);
		if (s[i].option)
			free(s[i].option);
		if (s[i].arg)
			free_2d(s[i].arg);
		if (s[i].dlm)
			free_2d(s[i].dlm);
		++i;
	}
	i = -1;
	free(s);
}

int	only_digits(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int check_com(char *com)
{
	int i;

	i = 0;
	if (!com)
		return (1);
	while(com[i])
	{
		if(ft_isalpha(com[i]))
			return (1);
		++i;
	}
	return (0);
}