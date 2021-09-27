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
	return (count);
}

int	str_count(char *str, char *s)
{
	int	count;
	int	i;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] == s[0] && str[i + 1] == s[1])
			++count;
	return (count);
}

void	error_exit(int error)
{
	if (error == 1)
	{
		write(2, "Error\n", 7);
		exit(1);
	}
	else if (error == 2)
	{
		status = 1;
		write(2, "No such file or directory\n", 26);
		exit(2);
	}
	else if (error == 3)
	{
		write(2, "Wrong path\n", 11);
		exit(1);
	}
	else if (error == 4)
	{
		write(2, "Syntax error\n", 13);
		exit(258);
	}
	else if (error == 5)
	{
		write(2, "malloc:: Not enough space\n", 26);
		exit(1);
	}
	else if (error == 6)
	{
		write(2, "minishell: too many arguments\n", 30);
		exit(1);
	}
	else if (error == 127)
	{
		write(2, "Command not found\n", 18);
		exit(127);
	}
	else if (error == 126)
	{
		write(2, "Unable to execute command\n", 26);
		exit(126);
	}
	exit(0);
}

int	check(char *str, simple_com *s)
{
	if (str && (str[0] == '"' || str[0] == '\''))
	{
		if (s->command == NULL && str)
			return (1);
		else if (str)
			return (3);
	}
	if (str && str[0] == 2)
	{
		if (s->command == NULL && str)
			return (8);
		else if (str)
			return (9);
	}
	if (str && str[1] && str[0] == '>' && str[1] == '>')
		return (7);
	else if (str && str[1] && ((str[0] == '<' && str[1] == '<') || (str[2] && str[0] == 3 && str[1] == '<' && str[2] == '<')))
		return (6);
	else if (str && str[0] == '<')
		return (4);
	else if (str && str[0] == '>')
		return (5);
	else if (str && str[0] == '-' && s->arg == NULL)
		return (2);
	else if (s->command == NULL && str)
		return (1);
	else if (str)
		return (3);
	return (-1);
}

void	fill_null(char **arr, int c)
{
	while (c-- > 0)
		arr[c] = NULL;
}

void	fill_fd(int *arr, char *str, int flag)
{
	static int count = 0;

	if (*arr != 0 && *arr != 1)
		close(*arr);
	if (flag == 4)
		*arr = open(str, O_RDONLY);
	else if (flag == 5)
		*arr = open(str, O_WRONLY | O_RDONLY | O_TRUNC | O_CREAT | S_IRUSR
			| S_IWUSR | S_IRGRP | S_IROTH , 0644);
	else if (flag == 7)
		*arr = open(str, O_WRONLY | O_RDONLY | O_APPEND | O_CREAT | S_IRUSR
			| S_IWUSR | S_IRGRP | S_IROTH, 0644);
	if (*arr == -1)
	{
		write(2, "Could not open file\n", 21);
		status = 1;
		return ;
	}
	++count;
}

char	*add_front(char *str, char c)
{
	int	len;
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;	
	len = ft_strlen(str);
	if (!(ret = malloc(sizeof(char) * (len + 2))))
		error_exit(5);
	ret[i] = c;
	i++;
	while (j < len)
		ret[i++] = str[j++];
	ret[i++] = '\0';
	return (ret);
}

int		compare(char *s1, char *s2)
{
	int	l1;
	int	l2;

	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	if (l1 != l2)
		return (0);
	while (l1 > 0)
	{
		if (*s1 != *s2)
			return (0);
		s1++;
		s2++;
		l1--;
	}
	return (1);
}

int	special_char(char *str)
{
	int		i;
	int		j;
	char 	c[] = {'>', '<', '$', '|'};
	char	*c1[] = {">>", "<<", "2>"};

	i = 0;
	j = 0;
	while (i < 4 && char_count(str, c[i]) == 0)
		i++;
	while (j < 3 && str_count(str, c1[j]) == 0)
	{
		i++;
		j++;
	}	
	if (i == 7)
		return (0);
	else
		return (1);
}

int	is_builtin(char *com)
{
	int	i;
	char	*c[7] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};

	i = 0;
	while (i < 7 && ft_strncmp(com, c[i], ft_strlen(c[i]) + 1))
		i++;
	if (i == 7)
		return (1);
	return (0);
}

char **create_arg(char **s, char *str, int flag)
{
	int		size;
	char	**arg;
	int		i;

	i = 0;
	if (s)
	{
		size = env_size(s);
		arg = malloc(sizeof(char *) * (size + 2));
		if (!arg)
			error_exit(5);
		i = -1;
		while (++i < size)
			arg[i] = ft_strdup(s[i]);
	}
	else
		if (!(arg = malloc(sizeof(char *) * 2)))
			error_exit(5);
	arg[i] = ft_strdup(str);
	arg[i+1] = NULL;
	if (flag == 1 && s)
		free_2d(s);
	return (arg);
}
