#include "minishell.h"

int	exec_com(simple_com *s, int fd[2])
{
	int status;

	// if (is_builtin(s->command) && !fd)
	// {

	// }
	// if (is_builtin(s->command)) //not a builtin
	// {
		write(2, "dup\n", 4);
		dup2(s->infile, STDIN_FILENO);
		write(2, "dup2\n", 5);
		dup2(s->outfile, STDOUT_FILENO);
		// close(fd[0]);
		// close(fd[1]);
		write(2, "Done dup\n", 9);
		// exit(0);
		if (execve(get_cmd_path(s), merge(s), s->env) == -1)
			write(2, "error\n", 6);
		exit(0);
	// }
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
	int		status;

	i = 0;
	// if (n == 1 && is_builtin(s->command))
	// 	exec_com(s, NULL);
	pipe(fd);
	pid = 1;
	while (pid != 0 && i < n)
	{
		ft_putstr_fd("PARENT id is:", 2);
		ft_putnbr_fd(getpid(), 2);
		ft_putstr_fd("\n", 2);
		pid = fork();

		write(1, "\n", 1);		
		if (pid == 0)
		{
			ft_putstr_fd("child id is:", 2);
			ft_putnbr_fd(getpid(), 2);
			ft_putstr_fd("\nparent id is:", 2);
			ft_putnbr_fd(getppid(), 2);
			ft_putstr_fd("\n", 2);
			if (s[i + 1].command != NULL)
				s[i].outfile = fd[1];
			// else
			// 	close (fd[1]);
			if (s[i].infile == 0 && i != 0)
				s[i].infile = fd[0];
			// else
			// 	close (fd[0]);
			write(2, "exec com\n", 9);
			exec_com(&s[i], fd);
			////////------------- erkusn el arajini outputn en vercrel--------------------------
		}
		else
		{
			// if (i != n - 1)
			// {
			// 	write(2, "waiting\n", 8);
			ft_putstr_fd("waited child's id is:", 2);
			ft_putnbr_fd(pid, 2);
			ft_putstr_fd("\n", 2);
			//wait(NULL);
			waitpid(pid, NULL, 0);
			// ft_putnbr_fd(status, 2);
			write(2, "Done waiting\n", 13);
			// }
		}
		i++;
	}
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