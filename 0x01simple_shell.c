#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

/**
 * displayPrompt - Displays the shell prompt
 */
void displayPrompt(void);

/**
 * executeCommand - Executes a command with arguments
 * @command: The command to be executed
 * @arguments: Array of arguments for the command
 */
void executeCommand(char *command, char **arguments);

/**
 * main - Entry point of the simple shell program
 * Return: Always 0 (success)
 */
int main(void) {
    char input[MAX_INPUT_SIZE];
    char *arguments[MAX_INPUT_SIZE];
    char *token;

    while (1) {
	int i ;
        displayPrompt();

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        token = strtok(input, " ");

	i = 0;
        while (token != NULL) {
            arguments[i++] = token;
            token = strtok(NULL, " ");
        }
        arguments[i] = NULL;

        executeCommand(arguments[0], arguments);
    }

    return 0;
}

/**
 * displayPrompt - Displays the shell prompt
 */
void displayPrompt(void) {
    printf("#cisfun$ ");
}

/**
 * executeCommand - Executes a command with arguments
 * @command: The command to be executed
 * @arguments: Array of arguments for the command
 */
void executeCommand(char *command, char **arguments) {
    pid_t child_pid;
    int status;

    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        if (execvp(command, arguments) == -1) {
            perror("exec");
            exit(EXIT_FAILURE);
        }
    } else {
        waitpid(child_pid, &status, 0);
    }
}

