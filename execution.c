#include "minishell.h"

int	exec_com(simple_com *s, int *fd)
{
	// int status;

	// if (is_builtin(s->command) && !fd)
	// {

	// }
	if (is_builtin(s->command)) //not a builtin
	{
		// printf("%s\n", get_cmd_path(s));
		// printf("%s\n", s->arg);
		// dup2(s->infile, STDIN_FILENO);
		// dup2(s->outfile, STDOUT_FILENO);
		execve(get_cmd_path(s), merge(s), s->env);
	}
	// else 
	// {
	// 	status = call_command(s);
	// }
	return (0);
}

void	exec(simple_com *s, int n)
{
	int 	i;
	int		fd[2];
	pid_t	pid;

	i = 0;
	if (n == 1 && is_builtin(s->command))
		exec_com(s, NULL);
	pipe(fd);
	pid = 1;
	while (pid != 0 && i < n)
	{
		pid = fork();
		if (pid == 0)
		{
			if (s[i + 1].command != NULL)
				s[i].outfile = fd[0];
			if (s[i].infile != 0)
				s[i].infile = fd[1];
			exec_com(&s[i], fd);
		}
		else
			wait(NULL);
		i++;
	}
}

// int	call_command(simple_com *s)
// {
// 	int status;

// 	if (compare(s->command, "echo"))
// 		status = echo(s);
// 	else if (compare(s->command, "cd"))
// 		status = cd(s);
// 	else if (compare(s->command, "pwd"))
// 		status = pwd(s);
// 	else if (compare(s->command, "export"))
// 		status = export(s);
// 	else if (compare(s->command, "unset"))
// 		status = unset(s);
// 	else if (compare(s->command, "env"))
// 		status = env(s);
// 	else if (compare(s->command, "exit"))
// 		status = exit(s);
// }