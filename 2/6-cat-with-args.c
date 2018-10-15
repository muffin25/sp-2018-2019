#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000
#define FD_OFFSET 2

int main(int argc, char const* const* argv)
{
    for (int i = 1; i < argc; i++)
        open(argv[i], O_RDONLY);

    char buffer[BUFFER_SIZE];
    size_t count;
    if (argc >= 2)
    {
        for (int i = 1; i < argc; i++)
            while ((count = read(FD_OFFSET + i, buffer, BUFFER_SIZE)) > 0)
                write(STDOUT_FILENO, buffer, count);
    }
    else
    {
        while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
            write(STDOUT_FILENO, buffer, count);
    }


    return 0;
}
