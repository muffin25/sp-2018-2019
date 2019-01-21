#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1000
#define MAX_STD_FILENO 2
#define NEWLINE_CHARACTER '\n'

#define REQUIRED_ARG_COUNT 1

void tail(const char* program_name, int fileno, size_t total_count)
{
    size_t current_count = 0;
    char buffer;
    ssize_t read_result;
    while (current_count < total_count - 1 && (read_result = read(fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_result)
        {
            perror(program_name);
            exit(3);
        }

        if (NEWLINE_CHARACTER == buffer)
            current_count++;
    }

    while (read(fileno, &buffer, 1) > 0)
        write(STDOUT_FILENO, &buffer, 1);
}

int main(int argc, char const* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    size_t total_count = atoi(argv[1]);
    if (total_count < 0)
    {
        perror(argv[0]);
        return 2;
    }

    if (argc > REQUIRED_ARG_COUNT + 1)
    {
        for (int i = REQUIRED_ARG_COUNT + 1; i < argc; i++)
        {
            if (-1 == open(argv[i], O_RDONLY))
            {
                perror(argv[0]);
                return 5;
            }
        }

        for (int i = 1; i < argc - REQUIRED_ARG_COUNT; i++)
            tail(argv[0], MAX_STD_FILENO + i, total_count);
    }
    else
        tail(argv[0], STDIN_FILENO, total_count);

    return 0;
}
