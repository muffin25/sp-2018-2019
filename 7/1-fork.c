#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, const char* const* argv)
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror(argv[0]);
        return 9;

    case 0:
        return 0;
    }

    int status;
    wait(&status);
    printf("%d\n", pid);
    printf("%d\n", WEXITSTATUS(status));
    return 0;
}