#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork");
        exit(9);

    case 0:
        exit(0);
    }

    int status;
    wait(&status);
    printf("%d\n", pid);
    printf("%d\n", WEXITSTATUS(status));

    return 0;
}