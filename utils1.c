#include "minishell.h"

char	**merge(simple_com *s)
{
	char	**r;
	int		size;
	int		i;
	int		j;

	i = 0;
	j = 0;
	size = 0;
	if (s->option)
		++size;
	r = malloc(sizeof(char *) * size);
	if (!r)
		error_exit(5);
	r[i++] = s->command;
	if (s->option)
		r[i++] = s->option;
	if (s->arg)
		while (s->arg[j])
			r[i++] = s->arg[j++];
	r[i] = NULL;
	return (r);
}

char *get_cmd_path(simple_com *s, t_env *e)
{
    int i = 0;
    char *command;
    char *path;
    int fd;
	char	**paths;

	if (!get_env("PATH", e))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(s->command, 2);
		ft_putendl_fd(": No such file or directory", 2);
		status = 127;
		exit(127);
	}
	while (e->myenv[i] && ft_strncmp(e->myenv[i], "PATH=", 5))
		++i;
	paths = ft_split(e->myenv[i], ':');
    if (ft_strchr(s->command, '/'))
		return (s->command);
	i = -1;
    while(paths[++i])
    {
        command = ft_strjoin("/", s->command, 0);
        path = ft_strjoin(paths[i], command, 0);
        fd = open(path, O_RDONLY);
        if (fd > -1)
        {
            close(fd);
            free(command);
			free_2d(paths);
            return(path);
        }
        free(command);
        free(path);
    }
    return (NULL);
}

int env_size(char **env)
{
	int size;

	if (!env)
		return (0);
	size = 0;
	while (env[size])
		size++;
	return (size);
}

void	free_2d(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	free(arr);
}

char **update_env(t_env *e, char *oldpath)
{
	char	*path;
	int		i;
	int		flag;
	int		size;
	char	**newenv;

	path = malloc(sizeof(char) * 1000);
	if (!path)
		error_exit(5);
	if (!getcwd(path, 1000))
		error_exit (3);
	i = 0;
	flag = 0;
	while (e->myenv[i])
	{
		if (ft_strncmp("OLDPWD=", e->myenv[i], 7) == 0)
		{
			free(e->myenv[i]);
			e->myenv[i] = ft_strdup(ft_strjoin("OLDPWD=", oldpath, 0));
			flag = 1;
		}
		++i;
	}
	i = 0;
	if (flag == 0)
	{
		size = env_size(e->myenv);
		newenv = malloc(sizeof(char *) * (size + 2));
		i = 0;
		while (i < size - 1)
		{
			newenv[i] = ft_strdup(e->myenv[i]);
			++i;
		}
		newenv[i++] = ft_strdup(ft_strjoin("OLDPWD=", path, 0));	
		newenv[i] = ft_strdup(e->myenv[size - 1]);
		newenv[i + 1] = NULL;
		free_2d(e->myenv);
		e->myenv = newenv;
	}
	i = 0;
	flag = 0;
	while (e->myenv[i])
	{
		if (ft_strncmp("PWD=", e->myenv[i], 4) == 0)
		{
			free(e->myenv[i]);
			e->myenv[i] = ft_strdup(ft_strjoin("PWD=", path, 0));
			flag = 1;
		}
		++i;
	}
	i = 0;
	if (flag == 0)
	{
		size = env_size(e->myenv);
		newenv = malloc(sizeof(char *) * (size + 2));
		i = 0;
		while (i < size - 1)
		{
			newenv[i] = ft_strdup(e->myenv[i]);
			++i;
		}
		newenv[i++] = ft_strdup(ft_strjoin("PWD=", path, 0));	
		newenv[i] = ft_strdup(e->myenv[size - 1]);
		newenv[i + 1] = NULL;
		free_2d(e->myenv);
		e->myenv = newenv;
	}
	return (e->myenv);
}

char *get_env(char *key, t_env *e)
{
	int		i;
	char	*temp;

	i = 0;
	if (key[0] == '?')
		return (ft_itoa(status));
	temp = ft_strjoin(key, "=", 0);
	while (e->myenv[i])
	{
		if (ft_strncmp(temp, e->myenv[i], ft_strlen(temp)) == 0)
		{
			free(temp);
			// if (*(e->myenv[i] + ft_strlen(temp)) == '\0')
			// 	return ("");
			return (e->myenv[i] + ft_strlen(temp));
		}
		++i;
	}
	free(temp);
	return (NULL);
}

char	*redir(char *str)
{
	char 	*s;
	int		len;

	if (str[0] && str[1] && ((str[0] == '2' && str[1] == '>') || (str[0] == '>' && str[1] == '>') || (str[0] == '<' && str[1] == '<')))
		len = 3;
	else if (str[0] && (str[0] == '<' || str[0] == '>'))
		len = 2;
	else
		return (NULL);
	s = malloc(sizeof(char) * len);
	if (!s)
		error_exit(5);
	if (len == 2)
		s[0] = str[0];
	else if (len == 3)
	{
		s[0] = str[0];
		s[1] = str[1];
	}
	s[len - 1] = 0;
	return (s);
}

char *remove_quote(char *str, t_env *e)
{
	char 	*s;
	char	*s1;
	int		flag;
	int 	len;
	int		qmark;
	int		q;
	int		q1;
	int		i;
	char 	*j;

	j = malloc(sizeof(char) * 2);
	if (!j)
		error_exit(5);
	j[1] = '\0';
	i = 0;
	q = -1;
	q1 = -1;
	s = malloc(2);
	flag = 0;
	qmark = -1;
	while (str[i])
	{
		if (str[i] && str[i] == '"' && q1 == -1)
		{
			if (ft_strncmp(str, "<<", 2) == 0)
				flag = 2;
			q *= -1;
		}
		else if (str[i] && str[i] == '\'' && q == -1)
		{
			if (ft_strncmp(str, "<<", 2) == 0)
				flag = 2;
			q1 *= -1;
		}
		else if (str[i] && str[i] == '$' && q1 == -1)
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
				if (!str[i] || str[i] == ' ')
				{
					j[0] = str[i - 1];
					s = ft_strjoin(s, j, 1);
					i--;
				}
				else
				{
					s = ft_strjoin(s, "", 0);
				}
			}
			else
			{
				if (!get_env(ft_substr(str, i - len, len), e))
				{
					s = ft_strjoin(s, "", 1); 
				}
				else
				{
					s = ft_strjoin(s, get_env(ft_substr(str, i - len, len), e), 1);
				}
				if (qmark != -1)
					i--;
			}
		}
		else
		{
			j[0] = str[i];
			s = ft_strjoin(s, j, 1);
		}
		if (is_redir(str + i) && (q == 1 || q1 == 1))
			flag = 1;
		i++;
	}
	free(j);
	if (flag == 1)
	{
		i = 0;
		s1 = malloc((ft_strlen(s) + 2) * sizeof(char));
		if (!s1)
			error_exit(5);
		s1[i] = 2;
		while (s[i])
		{
			s1[i + 1] = s[i];
			i++;
		}
		i++;
		s1[i] = 0;
		free(s);
		return (s1);
	}
	else if (flag == 2)
	{
		i = 0;
		s1 = malloc((ft_strlen(s) + 2) * sizeof(char));
		if (!s1)
			error_exit(5);
		s1[i] = 3;
		while (s[i])
		{
			s1[i + 1] = s[i];
			i++;
		}
		i++;
		s1[i] = 0;
		free(s);
		free(str);
		return (s1);
	}
	free(str);
	return (s);
}

int		is_redir(char *s)
{
	if (s[0] == '>' || s[0] == '<' || (s[0] == '2' && s[1] == '>')
		|| (s[0] == '>' && s[1] == '>') || (s[0] == '<' && s[1] == '<'))
		return (1);
	return (0);
}