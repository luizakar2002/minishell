#include "minishell.h"

int	exec_com(simple_com *s, int fd[2])
{
	int status;

	// if (is_builtin(s->command) && !fd)
	// {

	// }
	// if (is_builtin(s->command)) //not a builtin
	// {
		dup2(s->infile, STDIN_FILENO);
		dup2(s->outfile, STDOUT_FILENO);
		if (execve(get_cmd_path(s), merge(s), s->env) == -1)
			perror("lsh");
		exit(0);
	//}
	// else 
	// {
	// 	status = call_command(s);
	// }
	return (0);
}

void	exec(simple_com *s, int n)
{
	int 	i;
	int		j;
	int		fd[n][2];
	int		pids[n];
	int		status;

	i = 0;
	while (i < n + 1)
	{
		if (pipe(fd[i]) == -1)
			error_exit(6);
		i++;
	}

	i = 0;
	pids[i] = 1;
	while (pids[i] != 0 && i < n)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			error_exit(7);
		//child process		
		if (pids[i] == 0)
		{
			j = 0;
			while (j < n + 1)
			{
				if (i != j)
					close(fd[j][0]);
				if (i + 1 != j)
					close(fd[j][1]);
				j++;
			}
			if (s[i + 1].command != NULL)
				s[i].outfile = fd[i + 1][1];
			if (s[i].infile == 0 && i != 0)
				s[i].infile = fd[i][0];
			exec_com(&s[i], fd[i]);	
		}
		else
		{
			//wait(NULL);
			
			waitpid(pids[i], NULL, 0);
			close(fd[i + 1][1]);

		}
		i++;
	}
	// printf("closing\n");
	// close(fd[0]);
	// close(fd[1]);
}

// int	call_command(simple_com *s)
// {
// 	int status;

// 	if (compare(s->command, "echo"))
// 		status = echo(s);
// 	// else if (compare(s->command, "cd"))
// 	// 	status = cd(s);
// 	// else if (compare(s->command, "pwd"))
// 	// 	status = pwd(s);
// 	// else if (compare(s->command, "export"))
// 	// 	status = export(s);
// 	// else if (compare(s->command, "unset"))
// 	// 	status = unset(s);
// 	// else if (compare(s->command, "env"))
// 	// 	status = env(s);
// 	// else if (compare(s->command, "exit"))
// 	// 	status = exit(s);
// 	return (status);
// }
