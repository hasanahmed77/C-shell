#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_WORDS 1024
#define MAX_TOKEN 64

int main()
{
    char command[MAX_WORDS];
    char *args[MAX_TOKEN];

    while (1)
    {
        printf("NinjaShell> ");

        fgets(command, sizeof(command), stdin);

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
            break;

        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
                fprintf(stderr, "Expected argument to \"cd\"\n");
            else
            {
                if (chdir(args[1]) != 0)
                    perror("chdir failed");
            }
            continue;
        }

        pid_t pid = fork();

        if (pid < 0)
            perror("Fork failed!");
        else if (pid == 0)
        {
            if (execvp(args[0], args) == -1)
            {
                perror("Execution failed!");
            }
            exit(EXIT_FAILURE);
        }
        else
            wait(NULL);
    }

    return 0;
}