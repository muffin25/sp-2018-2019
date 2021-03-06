#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_FILE_MODE 0644

#define REQUIRED_ARG_COUNT 4

int main(int argc, char* const* argv)
{
    if (argc < REQUIRED_ARG_COUNT + 1)
        exit(1);

    const char* set1 = argv[1];

    const char* set2 = argv[2];

    int output_fileno = creat(argv[3], DEFAULT_FILE_MODE);
    if (-1 == output_fileno)
    {
        perror("open");
        exit(5);
    }

    int input_fileno = open(argv[4], O_RDONLY);
    if (-1 == input_fileno)
    {
        perror("open");
        exit(5);
    }

    dup2(input_fileno, STDIN_FILENO);
    dup2(output_fileno, STDOUT_FILENO);

    if (-1 == execlp("tr", "tr", set1, set2, NULL))
    {
        perror("exec");
        exit(8);
    }

    return 0;
}