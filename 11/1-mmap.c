#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MESSAGE "Hello world"
#define MESSAGE_LENGTH sizeof(MESSAGE)

int main(void)
{
    char* message = mmap(NULL, MESSAGE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    switch (fork())
    {
    case -1:
        perror("fork");
        exit(9);

    case 0:
        strcpy(message, MESSAGE);
        exit(0);

    default:
        wait(NULL);
        write(STDOUT_FILENO, message, MESSAGE_LENGTH);
    }

    return 0;
}