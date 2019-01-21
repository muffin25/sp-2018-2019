#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1000
#define MAX_STD_FILENO 2
#define NEWLINE_CHARACTER '\n'

#define REQUIRED_ARG_COUNT 2

void cut(const char* program_name, int fileno, size_t from_position, size_t to_position)
{
    size_t current_position = 0;
    char buffer;
    ssize_t read_result;
    while ((read_result = read(fileno, &buffer, 1)) != 0)
    {
        if (-1 == read_result)
        {
            perror(program_name);
            exit(3);
        }

        if (NEWLINE_CHARACTER == buffer)
        {
            current_position = 0;
            write(STDOUT_FILENO, &buffer, 1);
        }
        else
        {
            current_position++;
            if (current_position >= from_position && current_position <= to_position)
                write(STDOUT_FILENO, &buffer, 1);
        }
    }
}

int main(int argc, char const* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        return 1;

    int from_position = atoi(argv[1]);
    if (from_position < 0)
    {
        perror(argv[0]);
        return 2;
    }

    int to_position = atoi(argv[2]);
    if (to_position < 0)
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
            cut(argv[0], MAX_STD_FILENO + i, from_position, to_position);
    }
    else
        cut(argv[0], STDIN_FILENO, from_position, to_position);

    return 0;
}
