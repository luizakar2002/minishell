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
		printf("Error\n");
		exit(1);
	}
	else if (error == 127)
	{
		printf("Command not found\n");
		exit(127);
	}
	else if (error == 126)
	{
		printf("Unable to execute command\n");
		exit(126);
	}
	else if (error == 3)
	{
		printf("Wrong path\n");
		exit(1);
	}
	else if (error == 4)
	{
		printf("Syntax error\n");
		exit(1);
	}
	else if (error == 5)
	{
		printf("malloc:: Not enough space\n");
		exit(1);
	}
	else if (error == 0)
		// dprintf(2, "everything is fine!\n");
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
	if (str && str[0] == '<')
		return (4);
	else if (str && str[0] == '>')
		return (5);
	else if (str && str[0] == '>' && str[1] == '>')
		return (7);
	else if (str && str[0] == '-')
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
	else if (flag == 6)
		*arr = open(str, O_WRONLY | O_RDONLY | O_TRUNC | O_CREAT | S_IRUSR
			| S_IWUSR | S_IRGRP | S_IROTH, 0644);
	else if (flag == 7)
		*arr = open(str, O_WRONLY | O_RDONLY | O_APPEND | O_CREAT | S_IRUSR
			| S_IWUSR | S_IRGRP | S_IROTH, 0644);
	// dprintf(2, "OPEN() RETURNED %d\n", *arr);
	// perror("MINISHELL");
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
		return (0); //no special chars
	else
		return (1);
}

int	is_builtin(char *com)
{
	int	i;
	char	*c[7] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};

	i = 0;
	while (i < 7 && compare(com, c[i]) == 0)
	{
		i++;
	}
	if (i == 7)
		return (1);
	return (0); // builtin
}

char **create_arg(simple_com *s, char *str)
{
	int		size;
	char	**arg;
	int		i;

	i = 0;
	if (s->arg)
	{
		size = env_size(s->arg);
		arg = malloc(sizeof(char *) * (size + 2));
		if (!arg)
			error_exit(5);
		i = -1;
		while (++i < size)
		{
			if (!(arg[i] = malloc(sizeof(char) * (ft_strlen(s->arg[i]) + 1))))
				error_exit(5);
			arg[i] = s->arg[i];
		}
	}
	else
		if (!(arg = malloc(sizeof(char *) * 2)))
			error_exit(5);
	if (!(arg[i] = malloc(sizeof(char) * (ft_strlen(str) + 1))))
		error_exit(5);
	arg[i] = str;
	arg[i+1] = NULL;
	return (arg);
	//free the other arg
}

void	print(simple_com *s)
{
	int	i;

	while (s->command)
	{
		i = 0;
		printf("command %s\n", s->command);
		printf("option %s\n", s->option);
		if (s->arg)
			while (s->arg[i])
				printf("arg %s\n", s->arg[i++]);
		printf("infile %d\n", s->infile);
		printf("outfile %d\n", s->outfile);
		printf("\n");
		++s;
	}
}
