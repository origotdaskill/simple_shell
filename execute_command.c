#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * execute_command - Executes the given command
 * @command: The command to execute
 * Return: 1 on success, 0 on failure
 */
int execute_command(char *command)
{
    pid_t pid;
    int status;
    int pipe_fd[2];
    char buffer[1024];
    ssize_t bytesRead;

     if (pipe(pipe_fd) == -1) {
        perror("simple_shell");
        return 1;
    }


    pid = fork();

    if (pid == 0)
    {
        close(pipe_fd[0]); 

        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            perror("simple_shell");
            close(pipe_fd[1]);
            exit(EXIT_FAILURE);
        }

        close(pipe_fd[1]);

        if (execlp(command, command, NULL) == -1)
        {
            perror("simple_shell");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("simple_shell");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return 1;
    }
    else
    {
        close(pipe_fd[1]);

        while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead);
        }

        close(pipe_fd[0]);

        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
