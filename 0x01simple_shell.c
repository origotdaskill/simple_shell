#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
    printf("#cisfun$ ");
    fflush(stdout);
}

/**
 * main - Simple shell program
 *
 * Return: Always 0
 */
int main(void)
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        pid_t pid;

        display_prompt();

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if ((pid = fork()) == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (execlp(input, input, (char *)NULL) == -1)
            {
                perror("exec");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                fprintf(stderr, "./hsh: %s: No such file or directory\n", input);
            }
        }
    }

    return 0;
}

