#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/**
 * custom_getline - Custom implementation of getline function
 *
 * Return: Dynamically allocated string containing user input
 */
char *custom_getline(void)
{
	static char buffer[BUFFER_SIZE];
	static size_t pos = 0;
	static ssize_t bytesRead = 0;

	char *line = NULL;
	size_t lineSize = 0;

	while (1)
	{
		if (pos == (size_t)bytesRead)
		{
			bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE);

			if (bytesRead <= 0)
			{
				if (lineSize > 0)
					return line;
				else
					return NULL;
			}

			pos = 0;
		}

		char currentChar = buffer[pos++];

		if (currentChar == '\n' || currentChar == EOF)
		{
			line[lineSize] = '\0';
			return line;
		}

		line = realloc(line, lineSize + 1);
		line[lineSize++] = currentChar;
	}
}

/**
 * main - Entry point of the program
 *
 * Return: Always 0
 */
int main(void)
{
	char *input;

	while ((input = custom_getline()) != NULL)
	{
		printf("Input: %s\n", input);
		free(input);
	}

	return 0;
}
