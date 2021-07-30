// #include "minishell.h"

// int main()
// {
// 	pid_t	pid;
// 	int fd[2];

// 	pipe(fd);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		close(fd[0]);
// 		write(fd[1], "hello\n", 6);
// 		close(fd[1]);
// 	}	
// 	else
// 	{
// 		wait(NULL);
// 		pid = fork();
// 		if (pid == 0)
// 		{

// 			char *str = malloc(6);
// 			read(fd[0], str, 6);
// 			printf("%s\n", str);
// 			close(fd[0]);
// 			write(fd[1], "bye\n", 4);
// 			close(fd[1]);
// 		}
// 		else
// 		{
// 			wait(NULL);
// 			char *str = malloc(4);
// 			read(fd[0], str, 4);
// 			printf("%s\n", str);
// 			close(fd[0]);
// 			close(fd[1]);
// 		}
// 	}
// }