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

    pid = fork();

    if (pid == 0)
    {
        if (execlp(command, command, NULL) == -1)
        {
            perror("simple_shell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("simple_shell");
    }
    else
    {
        do
        {
            pid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
