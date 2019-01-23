#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_ARG_COUNT 100

int main(int argc, const char* const* argv)
{
    while (1)
    {
        write(1, "$ ", 2);
        char command_buffer[BUFSIZ];
        ssize_t command_length = read(0, command_buffer, BUFSIZ);
        if (-1 == command_length)
        {
            perror("read");
            exit(3);
        }

        command_buffer[command_length - 1] = '\0';

        char* command_argv[MAX_ARG_COUNT];
        command_argv[0] = strtok(command_buffer, " ");
        int command_argc = 0;
        do
        {
            command_argc++;
            command_argv[command_argc] = strtok(NULL, " ");
        }
        while (NULL != command_argv[command_argc]);

        if (0 == strcmp(command_argv[0], "exit") || 0 == strcmp(command_argv[0], "quit"))
            exit(0);

        pid_t pid = fork();
        if (-1 == pid)
        {
            perror("fork");
            exit(9);
        }
        else if (0 == pid && -1 == execvp(command_argv[0], command_argv))
        {
            fprintf(stderr, "%s: error: command `%s` does not exist\n", argv[0], command_argv[0]);
            exit(8);
        }
        else
        {
            int status;
            wait(&status);
            int exit_status = WEXITSTATUS(status);
            if (0 != exit_status)
                fprintf(stderr, "%s: warning: command `%s` (PID %d) exited with a non-zero status code (%d)\n", argv[0], command_argv[0], pid, exit_status);
        }
    }

    return 0;
}
