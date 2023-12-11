#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_PATH_SIZE 1024

/* Declare environ if not available */
extern char **environ;

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
 * commandExists - Checks if a command exists in the system's directories
 * @command: The command to check for existence
 * Return: 1 if the command exists, 0 otherwise
 */
int commandExists(char *command);

/**
 * builtinExit - Implements the exit built-in command
 */
void builtinExit(void);

/**
 * builtinEnv - Implements the env built-in command
 */
void builtinEnv(void);

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
            builtinExit();
            break;
        } else if (strcmp(input, "env") == 0) {
            builtinEnv();
            continue;
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
    printf(":) ");
}

/**
 * executeCommand - Executes a command with arguments
 * @command: The command to be executed
 * @arguments: Array of arguments for the command
 */
void executeCommand(char *command, char **arguments) {
    pid_t child_pid;
    int status;

    if (!commandExists(command)) {
        printf("%s: command not found\n", command);
        return;
    }

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

/**
 * commandExists - Checks if a command exists in the system's directories
 * @command: The command to check for existence
 * Return: 1 if the command exists, 0 otherwise
 */
int commandExists(char *command) {
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *token = strtok(path_copy, ":");

    while (token != NULL) {
        char full_path[MAX_PATH_SIZE];
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);

        if (access(full_path, F_OK) == 0) {
            free(path_copy);
            return 1; 
        }

        token = strtok(NULL, ":");
    }

    free(path_copy);
    return 0; 
}

/**
 * builtinExit - Implements the exit built-in command
 */
void builtinExit(void) {
    printf("Exiting the shell\n");
    exit(EXIT_SUCCESS);
}


/**
 * builtinEnv - Implements the env built-in command
 */
void builtinEnv(void) {
    char **env;
    for (env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}
