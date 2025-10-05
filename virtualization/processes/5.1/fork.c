#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc, char **argv)
{
    pid_t cp = fork();
    int status;
    if (cp < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }
    else if (cp == 0)
    {
        printf("Inside child process routine:%d\n", getpid());
        close(STDOUT_FILENO);
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        execvp(*(argv + 1), argv + 1);
    }
    else
    {
        wait(&status);
        printf("Inside parent process routine:%d\n", getpid());
    }
    return 0;
}