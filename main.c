#include "shell.h"
#include <stdio.h>
#include <string.h>


/**
 * main - Entry point of the simple shell
 * Return: 0 on success
 */
int main(void)
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        display_prompt();

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) > 0)
        {
            if (execute_command(input) == 0)
            {
                break;
            }
        }
    }

    return 0;
}
