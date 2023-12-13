#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 10

int main(void)
{
    char *input = NULL;
    size_t len = 0;
    ssize_t readBytes;

    while (1)
    {
        printf("$ ");
        fflush(stdout);

        readBytes = getline(&input, &len, stdin);

        if (readBytes == -1)
        {
            if (feof(stdin))
            {
                printf("\n");
                break;
            }
            else
            {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        if (input[readBytes - 1] == '\n')
        {
            input[readBytes - 1] = '\0';
        }

        char *token;
        char *args[MAX_ARGS];
        int argCount = 0;

        token = strtok(input, " ");
        while (token != NULL)
        {
            args[argCount++] = token;
            token = strtok(NULL, " ");
        }
        args[argCount] = NULL;

        if (access(args[0], X_OK) == 0)
        {
            if (fork() == 0)
            {
                execvp(args[0], args);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }
        }
        else
        {
            char *path = getenv("PATH");
            char *pathCopy = strdup(path);

            if (pathCopy == NULL)
            {
                perror("strdup");
                exit(EXIT_FAILURE);
            }

            token = strtok(pathCopy, ":");
            while (token != NULL)
            {
                char fullPath[1024];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", token, args[0]);

                if (access(fullPath, X_OK) == 0)
                {
                    if (fork() == 0)
                    {
                        execv(fullPath, args);
                        perror("execv");
                        exit(EXIT_FAILURE);
                    }
                    else
                    {
                        wait(NULL);
                        break;
                    }
                }

                token = strtok(NULL, ":");
            }

            free(pathCopy);

            printf("%s: command not found\n", args[0]);
        }
    }

    free(input);

    return EXIT_SUCCESS;
}
