#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

/**
 * main - Simple UNIX command line interpreter
 *
 * Return: Always 0.
 */
int main(void)
{
    char buffer[BUFFER_SIZE];
    ssize_t read_bytes;
    size_t len = 0;

    while (1)
    {
        printf("$ ");

        read_bytes = getline(&buffer, &len, stdin);

        if (read_bytes == -1)
        {
            if (feof(stdin))
            {
                printf("\n");
                break;
            }
            else
            {
                perror("Error reading input");
                exit(EXIT_FAILURE);
            }
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        pid_t pid = fork();

        if (pid == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            char *args[] = {buffer, NULL};
            if (execve(buffer, args, NULL) == -1)
            {
                perror("Command not found");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int status;
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("Error waiting for child process");
                exit(EXIT_FAILURE);
            }
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                fprintf(stderr, "Command failed with exit status %d\n", WEXITSTATUS(status));
            }
        }
    }

    free(buffer);

    return 0;
}

