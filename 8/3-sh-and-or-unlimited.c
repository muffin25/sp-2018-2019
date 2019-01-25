#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_ARG_COUNT 100

static const char* argv0;

int fork_exec(char* const* command_argv)
{
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(9);
    }
    else if (0 == pid && -1 == execvp(command_argv[0], command_argv))
    {
        fprintf(stderr, "%s: error: command `%s` does not exist\n", argv0, command_argv[0]);
        exit(8);
    }

    int status;
    wait(&status);
    int exit_status = WEXITSTATUS(status);
    if (0 != exit_status)
        fprintf(stderr, "%s: warning: command `%s` (PID %d) exited with a non-zero status code (%d)\n", argv0, command_argv[0], pid, exit_status);

    return WEXITSTATUS(status);
}

int main(int argc, const char* const* argv)
{
    argv0 = argv[0];

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

        int operator_count = 0, operator_types[MAX_ARG_COUNT], subcommand_argv_positions[MAX_ARG_COUNT];
        operator_types[operator_count] = 0;
        subcommand_argv_positions[0] = 0;
        for (int i = 0; i < command_argc; i++)
        {
            if (0 == strcmp(command_argv[i], "&&"))
            {
                operator_count++;
                operator_types[operator_count] = 1;
                subcommand_argv_positions[operator_count] = i + 1;
                command_argv[i] = NULL;
            }
            else if (0 == strcmp(command_argv[i], "||"))
            {
                operator_count++;
                operator_types[operator_count] = 2;
                subcommand_argv_positions[operator_count] = i + 1;
                command_argv[i] = NULL;
            }
        }
        operator_count++;

        int exit_status = 0;
        for (int i = 0; i < operator_count; i++)
        {
            if ((1 == operator_types[i] && 0 != exit_status) || (2 == operator_types[i] && 0 == exit_status))
                break;

            int subcommand_argv_position = subcommand_argv_positions[i];
            exit_status = fork_exec((char* const*)command_argv + subcommand_argv_position);
        }
    }

    return 0;
}
